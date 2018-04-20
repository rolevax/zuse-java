import QtQuick 2.0
import QtQuick.Dialogs 1.2
import sell 1.0

Rectangle {
    property string filename

    id: frame
    width: 800; height: 500
    color: "#222233"
    focus: true

    PDoc {
        id: pDoc

        onMessage: {
            messageDialog.text = text;
            messageDialog.open();
        }

        onTension: {
            showPad.setTension(b);
        }

        onPushed: {
            modesModel.append({ modelText: name });
        }

        onPopped: {
            modesModel.remove(modesModel.count - 1, 1);
        }

        onClipSwitched: { clipIndex.curr = c; }
        onInsertLine: { showPad.insertLine(r, ct); }
        onRemoveLine: { showPad.removeLine(r, ct); }
        onUpdateLine: { showPad.updateLine(r, str); }
        onHotLighted: { showPad.setHotLight(back); }
        onLighted: {
            showPad.light(false, lbr, lbc, ler, lec);
            showPad.light(true, hbr, hbc, her, hec);
        }
    }

    ShowPad {
        id: showPad
        anchors.fill: parent
    }

    ListView {
        id: modes
        model: ListModel {
            id: modesModel
        }

        width: 130
        height: parent.height
        anchors.right: parent.right
        verticalLayoutDirection: ListView.BottomToTop
        delegate: Rectangle {
            width: parent.width
            height: 35
            color: "yellow"
            Text {
                anchors.centerIn: parent
                text: modelText
                font.pixelSize: 20
            }
        }
    }

    Rectangle {
        property string curr: "a"

        id: clipIndex
        color: "#331199"
        width: 25
        height: 25
        anchors.top: parent.top
        anchors.right: parent.right

        Text {
            anchors.centerIn: parent
            text: parent.curr
            color: "white"
            font.pixelSize: 20
        }
    }

    MessageDialog {
        id: messageDialog
        icon: StandardIcon.Information
    }

    Keys.onPressed: {
        pDoc.keyboard(event.text, event.modifiers);
        event.accepted = true;
    }

    function load(fn) {
        filename = fn;
        pDoc.load(filename);
    }

    function save() {
        pDoc.save(filename);
    }
}

