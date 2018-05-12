import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    visible: true
    title: "Zuse-Java"
    width: 800; height: editor.height + openButton.height

    Button {
        id: openButton
        text: "Open"
        onClicked: {
            fileDialog.open();
        }
    }

    Text {
        anchors.left: openButton.right
        anchors.leftMargin: 10
        anchors.verticalCenter: openButton.verticalCenter
        text: editor.filename
        color: "white"
    }

    Button {
        id: saveButton
        text: "Save"
        anchors.right: parent.right
        onClicked: {
            editor.save();
        }
    }


    FileDialog {
        id: fileDialog
        title: "Please choose a source file"
        nameFilters: [ "Java source files (*.java)", "All files (*)" ]
        onAccepted: {
            editor.load(fileUrl);
        }
    }


    Editor {
        id: editor
        anchors.bottom: parent.bottom
    }
}
