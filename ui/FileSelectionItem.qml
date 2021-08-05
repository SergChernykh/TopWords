import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12

Item {
    id: root

    property string selectedFile

    signal fileSelected()

    QtObject {
        id: internal

        function fileUrlToLocalPath(fileUrl) {
            if (Qt.platform.os === "windows") {
                return fileUrl.replace(/^(file:\/{3})/,"")
            } else {
                return fileUrl.replace(/^(file:\/{2})/,"")
            }
        }
    }

    RowLayout {
        anchors.fill: parent

        spacing: 10 * appUIScale.xScale

        TextField {
            text: root.selectedFile
            readOnly: true
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
        }

        Button {
            text: qsTr("Select file")
            font.pixelSize: 16 * appUIScale.yScale
            onClicked: {
                fileDialog.open()
            }
            Layout.preferredHeight: 50 * appUIScale.yScale
            Layout.preferredWidth: 150 * appUIScale.xScale
        }
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Please choose a file")
        folder: shortcuts.home
        nameFilters: ["Text files (*.txt)"]
        visible: false
        onAccepted: {
            root.selectedFile = internal.fileUrlToLocalPath(fileDialog.fileUrl.toString())
            root.fileSelected()
            close();
        }
    }
}
