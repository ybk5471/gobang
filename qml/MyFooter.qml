import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

ToolBar {
	signal start()
	signal hint()
	signal giveUp()
	signal undo()
	property alias startBE: startB.enabled
	property alias hintBE: hintB.enabled
	property alias undoBE: undoB.enabled
	RowLayout {
		anchors.centerIn: parent
		Button {
			id: startB
			text: "开始"
			onClicked: start()
		}
		Button {
			id: hintB
			text: "提示"
			onClicked: hint()
		}
		Button {
			id: giveUpB
			enabled: !startB.enabled
			text: "认输"
			onClicked: giveUp()
		}
		Button {
			id: undoB
			text: "悔棋"
			onClicked: undo()
		}
	}
}
