import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2


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

    header: Item {
        FileSelectionItem {
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                margins: 10 * Math.min(appUIScale.xScale, appUIScale.yScale)
            }
            enabled: !counter.fileProcessing
            onFileSelected: {
                counter.processFile(selectedFile)
            }
        }
    }

    Connections {
        target: counter
        onError: {
            errorDialog.text = message
            errorDialog.open()
        }
    }

    MessageDialog {
       id: errorDialog
       icon: StandardIcon.Warning
       standardButtons: StandardButton.Ok
       visible: false
    }

    Item {
        id: body
        anchors.fill: parent

//        BarChart {
//            anchors {
//                centerIn: parent
//            }

//            width: 500
//            height: 300

//            model: wordsModel
//        }

        ProgressBar {
            id: progressBar
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
                leftMargin: 10 * appUIScale.xScale
                rightMargin: 10 * appUIScale.xScale
                bottomMargin: 40 * appUIScale.yScale
            }
            value: counter.progress
        }

        Text {
            anchors {
                top: progressBar.bottom
                topMargin: 10 * appUIScale.yScale
                horizontalCenter: progressBar.horizontalCenter
            }
            text: "%1 %".arg((progressBar.value * 100).toFixed(2))
        }
    }
}
