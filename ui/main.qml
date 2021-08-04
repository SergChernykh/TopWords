import QtQuick 2.14
import QtQuick.Controls 2.14

ApplicationWindow {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Stack")

    StackView {
        id: stackView
        initialItem: chartPageComponent
        anchors.fill: parent
    }

    Component {
        id: homePageComponent
        HomePage {

        }
    }

    Component {
        id: chartPageComponent
        ChartPage {

        }
    }
}
