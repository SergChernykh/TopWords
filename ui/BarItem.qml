import QtQuick 2.14
import QtQuick.Controls 2.14

Item {
    id: root

    property color color: "lightblue"
    property string label
    property int value

    Rectangle {
        anchors.fill: parent
        color: root.color

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
        }

        ToolTip.visible: mouseArea.containsMouse
        ToolTip.text: root.value
    }
}
