/*
   Copyright 2010 Unknown Author (Qt Centre)
   Copyright 2010 David Nolden <david.nolden.kdevelop@art-master.de>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "richtexttoolbutton.h"

#include <QPainter>
#include <QPixmap>
#include <QTextDocument>
#include <QIcon>
#include <QSize>
#include <QMenu>
#include <QStylePainter>
#include <qtextobject.h>
#include <QAbstractTextDocumentLayout>

using namespace KDevelop;

RichTextToolButton::RichTextToolButton(QWidget *parent) :
    QToolButton(parent)
{
}

void RichTextToolButton::setHtml(const QString &text)
{
    htmlText = text;
    isRichText = true;

    QPalette palette;
    palette.setBrush(QPalette::ButtonText, Qt::transparent);
    setPalette(palette);
}

void RichTextToolButton::setText(const QString &text)
{
    isRichText = false;
    QToolButton::setText(text);
}

QString RichTextToolButton::text() const
{
    if (isRichText) {
        QTextDocument richText;
        richText.setHtml(htmlText);
        return richText.toPlainText();
    } else
        return QToolButton::text();
}

QSize RichTextToolButton::sizeHint() const
{
    if(!isRichText) {
        return QToolButton::sizeHint();
    } else{
        QTextDocument richTextLabel;
        richTextLabel.setDefaultFont(font());
        richTextLabel.setHtml(htmlText);
        return richTextLabel.size().toSize();
    }
}

void RichTextToolButton::paintEvent(QPaintEvent *event)
{
    if (isRichText) {
        QStylePainter p(this);

        QRect buttonRect = rect();
        QPoint point;

        QTextDocument richTextLabel;
        richTextLabel.setDefaultFont(font());
        richTextLabel.setHtml(htmlText);

        QPixmap richTextPixmap(richTextLabel.size().width(), richTextLabel.size().height());
        richTextPixmap.fill(Qt::transparent);
        QPainter richTextPainter(&richTextPixmap);
        QAbstractTextDocumentLayout::PaintContext ctx;
        ctx.palette.setBrush(QPalette::Text, QColor("black"));
        ctx.clip=richTextPixmap.rect();
        richTextLabel.documentLayout()->draw(&richTextPainter, ctx);

        if (!icon().isNull())
            point = QPoint(buttonRect.x() + buttonRect.width() / 2 + iconSize().width() / 2 + 2, buttonRect.y() + buttonRect.height() / 2);
        else
            point = QPoint(buttonRect.x() + buttonRect.width() / 2 - 1, buttonRect.y() + buttonRect.height() / 2);

        buttonRect.translate(point.x() - richTextPixmap.width() / 2, point.y() - richTextPixmap.height() / 2);

        QStyleOptionButton opt = getStyleOption();
        p.drawControl(QStyle::CE_PushButtonBevel, opt);
        p.drawPrimitive(QStyle::PE_FrameFocusRect, opt);
        p.drawPixmap(buttonRect.topLeft(), richTextPixmap);
    } else
        QToolButton::paintEvent(event);
}

QStyleOptionButton RichTextToolButton::getStyleOption() const
{
    QStyleOptionButton opt;

    opt.initFrom(this);
    opt.features = QStyleOptionButton::None;
    if (menu())
        opt.features |= QStyleOptionButton::HasMenu;
    if (isDown() || (menu() && menu()->isVisible()))
        opt.state |= QStyle::State_Sunken;
    if (isChecked())
        opt.state |= QStyle::State_On;
    if (!isDown())
    {
        if(opt.state & QStyle::State_MouseOver && isEnabled())
            opt.state |= QStyle::State_Raised;
        else
            opt.features |= QStyleOptionButton::Flat;
    }
    opt.text = text();
    opt.icon = icon();
    opt.iconSize = iconSize();
    return opt;
}
