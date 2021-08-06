import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12

Item {
    id: root

    property color color
    property string value

    RowLayout {
        spacing: 10 * appUIScale.xScale
        anchors.fill: parent
        Rectangle {
            Layout.preferredWidth: 20 * Math.min(appUIScale.xScale, appUIScale.yScale)
            Layout.preferredHeight: 20 * Math.min(appUIScale.xScale, appUIScale.yScale)
            color: root.color
        }

        Text {
            Layout.fillWidth: true
            text: root.value
            fontSizeMode: Text.HorizontalFit
            elide: Text.ElideRight
        }
    }
}
