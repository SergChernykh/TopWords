import QtQuick 2.14
import QtQuick.Controls 2.14

Page {
    id: root
    width: 640
    height: 480

    Label {
        anchors.centerIn: parent
        text: qsTr("Home")
    }

    ProgressBar {
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 50
        }
        width: 300
        height: 10
        value: counter.progress
    }
}
