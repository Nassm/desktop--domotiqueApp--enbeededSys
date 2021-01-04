#include "burgermenu.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QString>
#include <QStyleOption>
#include <QPainter>
#include <QPropertyAnimation>

static const QString ButtonBurgerObjectName("BurgerButton");
static const QString MenuBurgerName("BurgerMenu");
static const QString MainButtonBurgerObjectName("MainBurgerButton");


/* class : déclaration d'une classe interne Button au MenuBurger */
class ButtonBurger : public QPushButton
{
    public: ButtonBurger(QAction* action, QWidget* parent) : QPushButton(parent), mIconSize(QSize(64,64)), mAction(action)
        {
            setObjectName(ButtonBurgerObjectName);
            connect(action, &QAction::destroyed, this, &ButtonBurger::deleteLater);
            setCursor(Qt::PointingHandCursor);

            connect(mAction, SIGNAL(changed()), this, SLOT(update()));
            connect(this, &ButtonBurger::clicked, [&]{
                if(mAction->isCheckable() && !mAction->isChecked())
                    mAction->toggle();

                mAction->trigger();
            });
        }

        void paintEvent(QPaintEvent*) override
        {
            QPainter painter(this);
            QStyleOptionButton opt;
            opt.initFrom(this);
            opt.state |= (mAction->isChecked() ? QStyle::State_On : QStyle::State_Off);

            style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
            if(!mAction->icon().isNull())
            {
                QIcon::Mode mode = ((opt.state & QStyle::State_MouseOver) == 0) ? QIcon::Normal : QIcon::Active;
                if(!isEnabled())
                    mode = QIcon::Disabled;
                QIcon::State state = mAction->isChecked() ? QIcon::On : QIcon::Off;
                painter.drawPixmap(QRect(QPoint(0,0), mIconSize), mAction->icon().pixmap(mIconSize, mode, state));
            }

            opt.rect = rect().adjusted(mIconSize.width(), 0, 0, 0);
            opt.text = fontMetrics().elidedText(mAction->iconText(), Qt::ElideRight, opt.rect.width(), Qt::TextShowMnemonic);
            style()->drawControl(QStyle::CE_CheckBoxLabel, &opt, &painter, this);
        }

        void setIconSize(const QSize& size) { mIconSize = size; setFixedHeight(mIconSize.height()); update(); }

        QAction* action() const { return mAction; }


        /* nos propriétés de classe */
    private:
        QSize mIconSize;
        QAction* mAction;
};


/* constructeur */
BurgerMenu::BurgerMenu(QWidget* parent): QWidget(parent), mActions(new QActionGroup(this)), mBurgerButton(new QPushButton(this)), mMenuWidth(200), mAnimated(true)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    mBurgerButton->setObjectName(MainButtonBurgerObjectName);
    mBurgerButton->setFlat(true);
    mBurgerButton->setIconSize(QSize(48,48));
    mBurgerButton->setFixedSize(48,48);
    mBurgerButton->setCheckable(true);
    mBurgerButton->setCursor(Qt::PointingHandCursor);
    mActions->setExclusive(true);

    auto burgerWidget = new QWidget(this);
    burgerWidget->setObjectName(MenuBurgerName);

    auto burgerLay = new QHBoxLayout();
    burgerLay->setContentsMargins(0, 0, 0, 0);
    burgerLay->setSpacing(0);
    burgerLay->addWidget(mBurgerButton);
    burgerLay->addWidget(burgerWidget);

    auto lay = new QVBoxLayout();
    setLayout(lay);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);

    lay->addLayout(burgerLay);
    lay->addStretch();
    setFixedWidth(48);

    /* initialisation des connexions dans l'instance */
    connect(mBurgerButton, &QPushButton::toggled, this, &BurgerMenu::setExpansionState);
    connect(mBurgerButton, &QPushButton::toggled, this, &BurgerMenu::expandedChanged);
    connect(mActions, &QActionGroup::triggered, this, &BurgerMenu::triggered);
}


/* func : retourne l'icone (ou image) du menu */
QIcon BurgerMenu::burgerIcon() const
{
    return mBurgerButton->icon();
}


/* func : retourne la taille de l'icone (ou image) du menu */
QSize BurgerMenu::iconSize() const
{
    return mBurgerButton->iconSize();
}


/* func : retourne la largeur du menu */
int BurgerMenu::menuWidth() const
{
    return mMenuWidth;
}


/* func : retourne la liste des actions (boutons) */
QList<QAction*> BurgerMenu::actions() const
{
    return mActions->actions();
}


/* slot : souscrit une action (bouton) */
QAction* BurgerMenu::addMenuAction(QAction* action)
{
    mActions->addAction(action);
    registerAction(action);
    return action;
}


/* slot : souscrit une action (bouton) */
QAction*BurgerMenu::addMenuAction(const QString& label)
{
    auto action = mActions->addAction(label);
    action->setCheckable(true);
    registerAction(action);
    return action;
}


/* slot : souscrit une action (bouton) */
QAction*BurgerMenu::addMenuAction(const QIcon& icon, const QString& label)
{
    auto action = mActions->addAction(icon, label);
    action->setCheckable(true);
    registerAction(action);
    return action;
}


/* slot : désinscrit une action (bouton) */
void BurgerMenu::removeMenuAction(QAction* action)
{
    mActions->removeAction(action);
    unRegisterAction(action);
}


/* slot : set l'icone (image) de nos boutons & emet un signal */
void BurgerMenu::setBurgerIcon(const QIcon& icon)
{
    mBurgerButton->setIcon(icon);
    emit iconChanged();
}


/* slot : set la taille des boutons & emet le signal */
void BurgerMenu::setIconSize(const QSize& size)
{
    if(size == mBurgerButton->iconSize())
        return;

    mBurgerButton->setIconSize(size);
    mBurgerButton->setFixedSize(size);
    auto buttons = findChildren<ButtonBurger*>(ButtonBurgerObjectName);
    for(auto btn : buttons)
        btn->setIconSize(size);

    if(mBurgerButton->isChecked())
        setFixedWidth(size.width() + mMenuWidth);
    else
        setFixedWidth(size.width());

    emit iconSizeChanged(size);
}


/* slot : set la valeur de mMenuWidth & emet le signal */
void BurgerMenu::setMenuWidth(int width)
{
    if(width == mMenuWidth)
        return;

    mMenuWidth = width;

    if(mBurgerButton->isChecked())
        setFixedWidth(mBurgerButton->iconSize().width() + mMenuWidth);
    else
        setFixedWidth(mBurgerButton->iconSize().width());

    emit menuWidthChanged(mMenuWidth);
}


/* gère l'animation de l'expansion du menu */
void BurgerMenu::setExpansionState(bool expanded)
{
    if(mAnimated)
    {
        auto anim = new QPropertyAnimation(this, "minimumWidth", this);
        anim->setDuration(250);
        anim->setStartValue(mBurgerButton->iconSize().width());
        anim->setEndValue(mBurgerButton->iconSize().width() + mMenuWidth);
        anim->setDirection(expanded ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        anim->setEasingCurve(expanded ? QEasingCurve::OutCubic : QEasingCurve::InCubic);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        if(expanded)
            setFixedWidth(mBurgerButton->iconSize().width() + mMenuWidth);
        else
            setFixedWidth(mBurgerButton->iconSize().width());
    }
}


/* inscrit une action via le bouton qui l'active */
void BurgerMenu::registerAction(QAction* action)
{
    auto button = new ButtonBurger(action, this);
    button->setIconSize(mBurgerButton->iconSize());
    auto lay = static_cast<QVBoxLayout*>(layout());
    lay->insertWidget(lay->count() - 1, button);
}


/* désinscrit une action via le bouton qui l'active */
void BurgerMenu::unRegisterAction(QAction* action)
{
    auto buttons = findChildren<ButtonBurger*>(ButtonBurgerObjectName);
    auto btn = std::find_if(buttons.begin(), buttons.end(), [&](ButtonBurger* btn){ return btn->action() == action; });
    if(btn != buttons.end())
        delete *btn;
}


/* get la valeur de mAnimated */
bool BurgerMenu::animated() const
{
    return mAnimated;
}


/* get la valeur de mExpanded */
bool BurgerMenu::expanded() const
{
    return mBurgerButton->isChecked();
}


/* slot : switch & set la valeur de mAnimated & emet un signal  */
void BurgerMenu::setAnimated(bool animated)
{
    if(mAnimated == animated)
        return;

    mAnimated = animated;
    emit animatedChanged(mAnimated);
}


/* set comme ckecked mExpanded */
void BurgerMenu::setExpanded(bool expanded)
{
    mBurgerButton->setChecked(expanded);
}


/* notre paint event*/
void BurgerMenu::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QStyleOption opt;
    opt.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
