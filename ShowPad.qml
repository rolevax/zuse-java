import QtQuick 2.5

ListView {
    property int highUpLeft;
    property int highUpRight;
    property int highUpIndex;
    property int highDownLeft;
    property int highDownRight;
    property int highDownIndex;
    property int lowUpLeft;
    property int lowUpRight;
    property int lowUpIndex;
    property int lowDownLeft;
    property int lowDownRight;
    property int lowDownIndex;

    model: ListModel {
        id: listModel
        ListElement {
            modelText: ""
        }
    }

    delegate: Row {
        width: parent.width
        height: 20
        objectName: "delegateItem"
        property int rowId: index
        spacing: 5

        Text { // line number
            text: "" + index
            color: "white"
            font.family: "Monospace"
            font.pixelSize: parent.height
            horizontalAlignment: Text.AlignRight
            width: 50
        }

        Item {
            height: parent.height
            width: 1 // just non-zero
            LightBar { // low light
                visible: lowUpIndex <= index && index <= lowDownIndex
                property bool up: index == lowUpIndex
                property bool down: index == lowDownIndex
                property bool mid: lowUpIndex < index && index < lowDownIndex
                lightColor: "#303040"
                leftOff: up ? lowUpLeft : lowDownLeft
                rightOff: up || mid ? lowUpRight : lowDownRight
            }

            LightBar { // high light
                visible: highUpIndex <= index && index <= highDownIndex
                property bool up: index == highUpIndex
                property bool down: index == highDownIndex
                property bool mid: highUpIndex < index && index < highDownIndex
                lightColor: "#443399"
                leftOff: up ? highUpLeft : highDownLeft
                rightOff: up || mid ? highUpRight : highDownRight
            }

            Text { // source text line
                text: modelText
                color: "white"
                font.family: "Monospace"
                font.pixelSize: parent.height
            }
        }
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

    /*
    LightBar {
        property bool tension: false
        id: highUp
        lightColor: tension ? "#22AA22" : "#443399"
    }
    */

    /*
    HotLight {
        id: hotLight
        leftOff: highUp.rightOff
        anchors.top: highUp.top
        width: 10; height: 20
        visible: false
    }
    */

    function setHotLight(back) {
        /*
        if (back < 0) {
            hotLight.visible = false;
        } else {
            hotLight.back = back;
            hotLight.visible = true;
        }
        */
    }

    function setTension(b) {
        /*
        highUp.tension = b;
        */
    }

    function light(high, br, bc, er, ec) {
        // suck tabs after bc
        bc = Math.max(bc, model.get(br).modelText.search(/\S/));

        // determine leftmost and rightmost non-space position
        var leftMost = bc;
        var rightMost = ec;
        for (var i = br; i <= er; i++) {
            var row = model.get(i).modelText;
            var nb = row.search(/\S/); // non blank
            if (i !== br && nb > 0)
                leftMost = Math.min(leftMost, nb);
            if (i !== er)
                rightMost = Math.max(rightMost, row.length);
        }

        if (high) {
            highUpLeft = bc;
            highUpRight = rightMost;
            highUpIndex = br;
            highDownLeft = leftMost;
            highDownRight = ec;
            highDownIndex = er;

            // position current line to center of window
            var windTop = contentY / 20; // TODO: de-magic
            var windBottom = (contentY + height) / 20;
            if (br < windTop || br > windBottom)
                positionViewAtIndex(br, ListView.Center);
        } else {
            lowUpLeft = bc;
            lowUpRight = rightMost;
            lowUpIndex = br;
            lowDownLeft = leftMost;
            lowDownRight = ec;
            lowDownIndex = er;
        }
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

