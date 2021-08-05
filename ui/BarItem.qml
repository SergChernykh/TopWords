import QtQuick 2.14
import QtQuick.Controls 2.14

Item {
    id: root

    property color color: "lightblue"
    property int value

    Rectangle {
        anchors.fill: parent
        color: root.color

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
        }
    }

    ToolTip {
        id: toolTip
        text: root.value
        visible: mouseArea.containsMouse
        contentItem: Text {
            text: toolTip.text
            font: toolTip.font
            color: "white"
        }
    }
}
