import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12

Item {
    id: root

    property alias model: view.model
    property int max: counter.frequencyAxisMax
    property int step: 10


    Column {
        anchors.fill: view

        spacing: height / root.step - 1

        Repeater {
            model: root.step + 1

            Item {
                width: parent.width
                height: 1

                Text {
                    id: stepValueText
                    anchors {
                        verticalCenter: parent.verticalCenter
                    }
                    x: -width
                    rightPadding: 5 * appUIScale.xScale
                    text: Math.floor(root.max / root.step * (root.step - index))
                    width: 80 * appUIScale.xScale
                    elide: Text.ElideLeft
                    fontSizeMode: Text.HorizontalFit

                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    color: "grey"
                }
            }
        }
    }

    ListView {
        id: view
        anchors {
            top: parent.top
            right: legendView.left
            rightMargin: 20 * appUIScale.xScale
            left: parent.left
            bottom: parent.bottom
        }

        orientation: ListView.Horizontal
        layoutDirection: ListView.LeftToRight
        interactive: false
        delegate: barItemDelegate
        focus: true
        spacing: 10 * appUIScale.xScale
    }

    ListView {
        id: legendView

        anchors {
            right: parent.right
            bottom: parent.bottom
            top: parent.top
        }

        width: 100 * appUIScale.xScale

        model: root.model
        interactive: false
        spacing: 5 * appUIScale.yScale
        delegate: legendItemDelegate

    }

    Component {
        id: legendItemDelegate

        LegendItem {
            readonly property ListView __lv: ListView.view

            width: __lv.width
            height: 20 * appUIScale.yScale

            value: model.word
            color: model.color
        }
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
            color: model.color
        }
    }
}
