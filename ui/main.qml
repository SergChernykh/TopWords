import QtQuick 2.14
import QtQuick.Controls 2.14

ApplicationWindow {
    id: window
    width: 720
    height: 560
    visible: true
    title: qsTr("Top words")
    minimumWidth: 720
    minimumHeight: 560

    Scale {
        id: appUIScale
        xScale: window.width / minimumWidth
        yScale: window.height / minimumHeight
    }

    StackView {
        id: stackView
        initialItem: homePageComponent
        anchors.fill: parent
    }

    Component {
        id: homePageComponent
        HomePage {
            id: homePage
            onFileSelected: {
                console.log("Selected file: ", path)
            }
        }
    }

    Component {
        id: chartPageComponent
        ChartPage {

        }
    }
}
