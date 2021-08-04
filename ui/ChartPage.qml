import QtQuick 2.14
import QtQuick.Controls 2.14

Page {
    id: root
    width: 640
    height: 480

    Label {
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 15
        }

        text: qsTr("Chart")
    }

//    ListModel {
//        id: wordsModel

//        Component.onCompleted: {
//            for (var i = 0; i < 15; i++) {
//                var item = {"word" : `word${i}`, "frequency" : 100 * (i + 1)}
//                wordsModel.append(item)
//            }
//        }
//    }

    BarChart {
        id: chart

        anchors {
            centerIn: parent
        }

        width: 500
        height: 300

        model: wordsModel
    }
}
