import QtQuick 2.14
import QtQuick.Controls 2.14

Item {
    id: root

    property alias model: view.model
    property int max: 2000
    property int step: 10


    Column {
        anchors.fill: parent

        spacing: height / root.step - 1

        Repeater {
            model: 10

            Item {
                width: parent.width
                height: 1

                Text {
                    id: stepValueText
                    anchors {
                        verticalCenter: parent.verticalCenter
                    }
                    x: -width
                    rightPadding: 5
                    text: Math.floor(root.max / 10 * (10 - index))
                    width: 50
                    fontSizeMode: Text.HorizontalFit

                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }

//                Rectangle {
//                    anchors {
//                        fill: stepValueText
//                    }
//                    color: "transparent"
//                    border.width: 1
//                    border.color: "red"
//                }

                Rectangle {
                    width: parent.width
                    height: 1
                    color: "grey"
                }
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: "grey"
        border.width: 1

        Text {
            anchors {
                verticalCenter: parent.bottom
            }
            x: -width
            rightPadding: 5
            text: "0"
            width: 50
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
    }

    ListView {
        id: view
        anchors.fill: parent
        orientation: ListView.Horizontal
        layoutDirection: ListView.LeftToRight
        interactive: false
        delegate: barItemDelegate
        focus: true
        spacing: 10
    }

    Component {
        id: barItemDelegate
        BarItem {
            readonly property ListView __lv: ListView.view

            anchors {
                bottom: parent.bottom
            }

            height: (value * __lv.height) / root.max
            width: (__lv.width - (__lv.count - 1) * __lv.spacing) / __lv.count
            value: model.frequency
            label: model.word

            Label {
                id: labelText
                anchors {
                    bottom: parent.bottom
                    bottomMargin: -50
                    horizontalCenter: parent.horizontalCenter
                }
                rotation: -90
                width: 50

                text: model.word
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter

            }
        }
    }
}
