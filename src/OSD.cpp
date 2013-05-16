/******************************************************************************
    QtAV:  Media play library based on Qt and FFmpeg
    Copyright (C) 2013 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QtAV

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
******************************************************************************/

#include "QtAV/OSD.h"
#include <QtAV/Statistics.h>

namespace QtAV {

OSD::OSD():
    mShowType(ShowCurrentAndTotalTime)
  , mSecsTotal(-1)
{
    mFont.setBold(true);
    mFont.setPixelSize(26);
}

OSD::~OSD()
{
}

void OSD::setShowType(ShowType type)
{
    mShowType = type;
}

OSD::ShowType OSD::showType() const
{
    return mShowType;
}

void OSD::useNextShowType()
{
    if (mShowType == ShowNone) {
        mShowType = (ShowType)1;
        return;
    }
    if (mShowType + 1 == ShowNone) {
        mShowType = ShowNone;
        return;
    }
    mShowType = (ShowType)(mShowType << 1);
}

bool OSD::hasShowType(ShowType t) const
{
    return (t&mShowType) == t;
}

void OSD::setFont(const QFont &font)
{
    mFont = font;
}

QFont OSD::font() const
{
    return mFont;
}

QString OSD::text(Statistics *statistics)
{
    //TODO: choose video or audio?
    QString text;
    if (hasShowType(ShowCurrentTime) || hasShowType(ShowCurrentAndTotalTime)) {
        text = statistics->video.current_time.toString("HH:mm:ss");
    }
    //how to compute mSecsTotal only once?
    if (hasShowType(ShowCurrentAndTotalTime) || hasShowType(ShowPercent) /*mSecsTotal < 0*/) {
        if (statistics->duration.isNull())
            return text;
        mSecsTotal = QTime(0, 0, 0).secsTo(statistics->duration); //why video.total_time may be wrong(mkv)
        //qDebug("secs=%d/%d", QTime(0, 0, 0).secsTo(statistics->video.current_time), mSecsTotal);
    }
    if (hasShowType(ShowCurrentAndTotalTime)) {
        text += "/" + statistics->duration.toString("HH:mm:ss");
    }
    if (hasShowType(ShowRemainTime)) {
        text += "-" + QTime().addSecs(statistics->video.current_time.secsTo(statistics->duration)).toString("HH:mm:ss");
    }
    if (hasShowType(ShowPercent))
        text += QString::number(qreal(QTime(0, 0, 0).secsTo(statistics->video.current_time))
                                /qreal(mSecsTotal)*100.0, 'f', 1) + "%";
    return text;
}


} //namespace QtAV
