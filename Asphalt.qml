import QtQuick 2.5

ListView {
    model: ListModel {
        id: listModel
        ListElement {
            modelText: ""
        }
    }

    delegate: Text {
        property int rowId: index

        objectName: "delegateItem"
        width: parent.width
        text: modelText
        color: "white"
        font.family: "Monospace"
        font.pixelSize: 20
    }

    function rowAt(id) {
        forceLayout();
        for (var i = 0; i < contentItem.children.length; i++) {
            var item = contentItem.children[i];
            if (item.objectName === "delegateItem" &&
                    item.rowId === id)
                return item;
        }
        return undefined;
    }

    function insertLine(r, ct) {
        for (var i = 0; i < ct; i++)
            listModel.insert(r, { modelText: "" });
    }

    function removeLine(r, ct) {
        listModel.remove(r, ct);
    }

    function updateLine(r, str) {
        listModel.get(r).modelText = str;
    }
}

