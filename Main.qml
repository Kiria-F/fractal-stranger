import QtQuick
import QtQuick.Controls
import io.qt.forum

Window {
    id: root
    visible: true
    title: qsTr("Fractal Stranger")
    width: 500
    height: 400
    property real scale: 80
    property real scaleOld: 80
    property real offsetX: 0
    property real offsetXOld: 0
    property real offsetY: 0
    property real offsetYOld: 0

    function redraw() {
        dynamicImage.stretch(root.scale / root.scaleOld, (root.offsetX - root.offsetXOld) * scale, (root.offsetY - root.offsetYOld) * scale)
        core.redraw(root.width, root.height, root.scale, root.offsetX, root.offsetY)
    }

    Component.onCompleted: core.redraw(root.width, root.height, root.scale, root.offsetX, root.offsetY)
    onWidthChanged: root.redraw()
    onHeightChanged: root.redraw()

    Connections {
        target: core

        function onRedrawFinished(image) {
            dynamicImage.setImage(image)
            root.offsetXOld = root.offsetX
            root.offsetYOld = root.offsetY
            root.scaleOld = root.scale
            progressBar.width = 0
        }

        function onSetProgress(value) {
            progressBar.width = value * root.width
        }
    }

    Rectangle {
        color: "black"
        anchors.fill: parent
    }

    DynamicImage {
        id: dynamicImage
        anchors.fill: parent
    }

    Rectangle {
        id: progressBar
        anchors {
            left: parent.left
            bottom: parent.bottom
        }
        color: "red"
        height: 1.5
        width: 0
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        property int pressX
        property int pressY
        property bool pressed
        property real offsetXOld
        property real offsetYOld

        onWheel: function(event) {
            let realAnchorX = (event.x - root.width / 2) / root.scale - root.offsetX
            let realAnchorY = (event.y - root.height / 2) / root.scale - root.offsetY
            if (event.angleDelta.y >= 0) {
                root.scale *= (event.angleDelta.y / 200) + 1
            } else {
                root.scale /= (-event.angleDelta.y / 200) + 1
            }
            root.offsetX = (event.x - root.width / 2) / root.scale - realAnchorX
            root.offsetY = (event.y - root.height / 2) / root.scale - realAnchorY

            root.redraw()
        }

        onPressed: function(event) {
            mouseArea.pressed = true
            mouseArea.pressX = event.x
            mouseArea.pressY = event.y
            mouseArea.offsetXOld = root.offsetX
            mouseArea.offsetYOld = root.offsetY
        }

        onMouseXChanged: function(event) {
            if (mouseArea.pressed) {
                root.offsetX = mouseArea.offsetXOld + (event.x - mouseArea.pressX) / root.scale
            }
            root.redraw()
        }

        onMouseYChanged: function(event) {
            if (mouseArea.pressed) {
                root.offsetY = mouseArea.offsetYOld + (event.y - mouseArea.pressY) / root.scale
            }
            root.redraw()
        }

        onReleased: function(event) {
            mouseArea.pressed = false
        }

        onClicked: function(event) {
            let t_offsetXOld = root.offsetX
            let t_offsetYOld = root.offsetY
            let t_scaleOld = root.scale
            let t_realAnchorX = (event.x - root.width / 2) / root.scale - root.offsetX
            let t_realAnchorY = (event.y - root.height / 2) / root.scale - root.offsetY
        }
    }
}
