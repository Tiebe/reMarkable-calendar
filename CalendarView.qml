import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.remarkable
import device.global
import device.ui.text
import device.ui.controls
import device.ui.headers
import device.view.dialogs
import device.view.documentview
import device.view.navigator
import common
import xofm.libs.orientation

import ark.controls as ArkControls

import dev.tiebe.Calendar 1.0

DeviceKeyboardNavigationHandler {
    id: calendarView
    objectName: "CalendarView"
    navigationModel: []
    canAcceptTabActivation: false
    canTakeFirstNavigationFocus: false

    property PopupOverlay popupOverlay
    property PageSelection movePageSelection

    property var pageSelection // Not used.. just to suppress warnings

    Component.onCompleted: {
        calendar.loadCalendars();
    }

    Calendar {
        id: calendar
    }

    Background {
        anchors.fill: parent
        color: Values.colorWhite
    }

    Rectangle {
        anchors.fill: header
        z: header.z
    }

    Column {
        id: header
        width: parent.width
        spacing: 20
        anchors.top: parent.top
        z: 10

        Item {
            height: 10
            width: parent.width
        }

        ArkControls.Title {
            type: ArkControls.Title.Large
            id: title
            text: qsTr("Calendar")
            anchors {
                left: parent.left
                right: parent.right
                leftMargin: Values.designGridMargin * 2
                rightMargin: Values.designGridMargin * 2
            }

            RowLayout {
                visible: contentDropdown.key == "month"
                width: 400
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                IconButton {                    
                    Layout.alignment: Qt.AlignVCenter
                    iconSource: "qrc:/ark/icons/angle_left"
                    onClicked: { calendar.subtractMonth() }
                }

                TextParagraph {
                    text: qsTr(Qt.formatDate(calendar.date, "MMMM"))

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                }
                
                IconButton {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.leftMargin: 15

                    iconSource: "qrc:/ark/icons/angle_right"
                    onClicked: { calendar.addMonth() }
                }
            }

            RowLayout {
                visible: contentDropdown.key == "week"
                width: 400
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                IconButton {                    
                    Layout.alignment: Qt.AlignVCenter
                    iconSource: "qrc:/ark/icons/angle_left"
                    onClicked: { calendar.subtractWeek() }
                }

                TextParagraph {
                    function getWeek(date, dowOffset) {
                    /*getWeek() was developed by Nick Baicoianu at MeanFreePath: http://www.meanfreepath.com */

                        dowOffset = typeof(dowOffset) == 'number' ? dowOffset : 0; //default dowOffset to zero
                        var newYear = new Date(date.getFullYear(),0,1);
                        var day = newYear.getDay() - dowOffset; //the day of week the year begins on
                        day = (day >= 0 ? day : day + 7);
                        var daynum = Math.floor((date.getTime() - newYear.getTime() - 
                        (date.getTimezoneOffset()-newYear.getTimezoneOffset())*60000)/86400000) + 1;
                        var weeknum;
                        //if the year starts before the middle of a week
                        if(day < 4) {
                            weeknum = Math.floor((daynum+day-1)/7) + 1;
                            if(weeknum > 52) {
                                nYear = new Date(date.getFullYear() + 1,0,1);
                                nday = nYear.getDay() - dowOffset;
                                nday = nday >= 0 ? nday : nday + 7;
                                /*if the next year starts before the middle of
                                the week, it is week #1 of that year*/
                                weeknum = nday < 4 ? 1 : 53;
                            }
                        }
                        else {
                            weeknum = Math.floor((daynum+day-1)/7);
                        }
                        return weeknum;
                    }


                    text: qsTr("Week " + getWeek(calendar.date, 0) + Qt.formatDate(calendar.date, " (yyyy)"))

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                }
                
                IconButton {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.leftMargin: 15

                    iconSource: "qrc:/ark/icons/angle_right"
                    onClicked: { calendar.addWeek() }
                }
            }

            ContentDropdown {
                id: contentDropdown
                name: "calendarViewDropdown"
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    topMargin: -20
                }
                values: [qsTr("Month"), qsTr("Week"), qsTr("Day")]
                keys: ["month", "week", "day"]
                key: keys[1]
                orientation: calendarView.orientation
                onSelected: (selectedKey) => {
                    console.log(selectedKey);
                    contentDropdown.key = selectedKey;
                }
                expandedText: qsTr("View")
                showViewSettings: false
            }
        }
    }


    RowLayout {
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: 50
        anchors.bottomMargin: 50
        width: parent.width
        visible: contentDropdown.key == "month"
        
        WeekNumberColumn {
            month: grid.month
            year: grid.year
            locale: grid.locale
            Layout.fillHeight: true

            delegate: TextParagraph {
                text: weekNumber
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                width: 50

                required property int weekNumber

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        contentDropdown.key = "week";
                    }
                }
            }
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            DayOfWeekRow {
                Layout.fillWidth: true
                locale: grid.locale
            }

            MonthGrid {
                id: grid
                Layout.fillWidth: true
                Layout.fillHeight: true
                month: calendar.date.getMonth()
                year: calendar.date.getFullYear()
                delegate: Column {
                    opacity: model.month === grid.month ? 1 : 0.5
                    TextParagraph {
                        width: parent.width
                        horizontalAlignment: Text.AlignHCenter
                        text: model.day
                    }

                    DateFilterProxyModel {
                        id: dateProxy
                        sourceModel: calendar.eventModel
                        filterDate: new Date(model.year, model.month, model.day)
                        filterByWeek: false
                    }

                    
                    Repeater {
                        id: dayEventRepeater
                        model: dateProxy

                        delegate: Rectangle {
                            id: calendarEventItem

                            color: "#E6F4EA"
                            radius: 6
                            border.color: "#98D4B9"
                            border.width: 1
                            clip: true
                            width: parent.width
                            height: 40

                            RowLayout {
                                id: layout
                                anchors.fill: parent
                                anchors.margins: 6

                                TextParagraph {
                                    id: textSummary
                                    text: model.summary
                                    font.bold: true
                                    color: "#0f0f0f"
                                    wrapMode: Text.NoWrap
                                    elide: Text.ElideRight
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }

                    required property var model
                }

                onClicked: function (date) {
                    console.log(date);
                    contentDropdown.key = "day";
                }
            }
        }
    }

    function getStartOfWeek(d) {
        var temp = new Date(d.getFullYear(), d.getMonth(), d.getDate())
        var dayOfWeek = temp.getDay()

        if (dayOfWeek === 0) {
            temp.setDate(temp.getDate() - 6)
        } else {
            temp.setDate(temp.getDate() - (dayOfWeek - 1))
        }
        return temp
    }

    function timeToMinutes(jsDate) {
        if (!jsDate)
            return 0
        return jsDate.getHours() * 60 + jsDate.getMinutes()
    }

    function dayFromStartOfWeek(eventDate, startOfWk) {
        if (!eventDate || !startOfWk)
            return 0


        let msInDay = 24*60*60*1000
        let diff = eventDate.getTime() - startOfWk.getTime()
        let day = Math.floor(diff / msInDay)
        return day
    }

    Rectangle {
        id: weekView
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        width: parent.width
        visible: contentDropdown.key == "week"


        property var hourHeight: 200
        property var hourWidth: 60
        property int scrollToHour: 8
        property var startOfWeek: getStartOfWeek(calendar.date)

        Rectangle {
            id: daysHeader
            z: 10
            width: parent.width
            
            height: 70
            color: "white"
            Row {
                id: days
                anchors.fill: parent
                spacing: 0
                anchors.topMargin: 20
                anchors.leftMargin: weekView.hourWidth

                Repeater {
                    id: dayRepeater
                    model: 7 // We want 7 days in a week

                    delegate: TextParagraph {  
                        // Optional: center the text within each cell
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        width: daysArea.width / 7

                        property var dayDate: {
                            var base = getStartOfWeek(calendar.date)
                            base.setDate(base.getDate() + index)
                            return base
                        }

                        text: {
                            var wd = Qt.locale().dayName(index + 1, 1)
                            var dd = ("0" + dayDate.getDate()).slice(-2)
                            var mm = ("0" + (dayDate.getMonth() + 1)).slice(-2)
                            return wd + " (" + dd + "-" + mm + ")"
                        }
                    }
                }
            }
        }

        Flickable {
            id: flickable
            anchors.top: daysHeader.bottom
            anchors.bottom: parent.bottom
            width: parent.width

            Component.onCompleted: {
                contentY = weekView.scrollToHour * weekView.hourHeight
            }

            contentHeight: contentItem.childrenRect.height
            contentWidth: contentItem.childrenRect.width

            Row {
                id: mainRow

                ColumnLayout {
                    id: hoursColumn
                    spacing: 0
                    width: weekView.hourWidth

                    Repeater {
                        model: 24
                        delegate: Rectangle {
                            width: parent.width
                            height: weekView.hourHeight
                            border.width: 1
                            border.color: "lightgrey"

                            Text {
                                anchors.centerIn: parent
                                text: ("0" + index).slice(-2) + ":00"
                            }
                        }
                    }
                }

                Item {
                    id: daysArea
                    width: flickable.width - hoursColumn.width
                    height: 24 * weekView.hourHeight

                    GridLayout {
                        id: weekGrid
                        anchors.fill: parent
                        columns: 7
                        rowSpacing: 0
                        columnSpacing: 0

                        Repeater {
                            model: 7
                            delegate: ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 0

                                Repeater {
                                    model: 24
                                    delegate: Rectangle {
                                        Layout.fillWidth: true
                                        height: weekView.hourHeight
                                        border.width: 1
                                        border.color: "lightgrey"
                                        color: "transparent"
                                    }
                                }
                            }
                        }
                    }            

                    DateFilterProxyModel {
                        id: dateProxy
                        sourceModel: calendar.eventModel
                        filterDate: calendar.date
                        filterByWeek: true
                    }

                    Repeater {
                        id: eventRepeater
                        model: dateProxy
                        Rectangle {
                            color: "lightblue"
                            border.color: "blue"
                            radius: 4

                            property int startMinutes: timeToMinutes(model.dtStart)
                            property int endMinutes: timeToMinutes(model.dtEnd)
                            property int dayIndex: dayFromStartOfWeek(model.dtStart, weekView.startOfWeek)

                            width: daysArea.width / 7 
                            x: dayIndex * (daysArea.width / 7)
                            y: startMinutes * (weekView.hourHeight / 60.0)
                            height: Math.max(10,
                                             (endMinutes - startMinutes) * (weekView.hourHeight / 60.0))

                            TextParagraph {
                                anchors.centerIn: parent
                                anchors.fill: parent
                                text: model.summary
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                }
            }
        }
    }
}
