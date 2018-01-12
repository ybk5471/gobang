import QtQuick 2.9

Rectangle {
	width: 32
	height: width
	color: "transparent"
	signal triggered()
	property alias leftV:		left.visible
	property alias rightV:		right.visible
	property alias upV:		up.visible
	property alias downV:		down.visible
	property alias centerV:		center.visible
	property alias chessmanV:	chessman.visible
	property alias chessmanC:	chessman.color
	property alias buttonV:		button.visible
	Rectangle {
		id: left
		width: parent.width / 2
		height: 1
		color: "black"
		anchors.left: parent.left
		anchors.verticalCenter: parent.verticalCenter
	}
	Rectangle {
		id: right
		width: parent.width / 2
		height: 1
		color: "black"
		anchors.right: parent.right
		anchors.verticalCenter: parent.verticalCenter
	}
	Rectangle {
		id: up
		width: 1
		height: parent.width / 2
		color: "black"
		anchors.top: parent.top
		anchors.horizontalCenter: parent.horizontalCenter
	}
	Rectangle {
		id: down
		width: 1
		height: parent.width / 2
		color: "black"
		anchors.bottom: parent.bottom
		anchors.horizontalCenter: parent.horizontalCenter
	}
	Rectangle {
		id: center
		width: parent.width / 4
		height: width
		color: "black"
		anchors.centerIn: parent
	}
	Rectangle {
		id: chessman
		width: parent.width * 2 / 3
		height: width
		color: "black"
		radius: width / 2
		anchors.centerIn: parent
	}
	MouseArea {
		id: button
		enabled: !chessman.visible
		anchors.fill: parent
		onClicked: {
			triggered()
		}
	}

	function click() {
		triggered()
	}
}
