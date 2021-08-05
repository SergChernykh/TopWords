import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Dialogs 1.2


Page {
    id: root

    anchors.fill: parent

    signal fileSelected(string path)

    FileDialog {
        id: fileDialog
        title: qsTr("Please choose a file")
        folder: shortcuts.home
        nameFilters: ["Text files (*.txt)"]
        visible: false
        onAccepted: {
            root.fileSelected(fileDialog.fileUrl)
            close();
        }
    }

    Button {
        anchors.centerIn: parent
        text: qsTr("Select file")
        font.pixelSize: 16 * appUIScale.yScale
        onClicked: {
            fileDialog.open()
        }
        height: 100 * appUIScale.yScale
        width: 200 * appUIScale.xScale
    }
}
