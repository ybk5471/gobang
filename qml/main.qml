import QtQuick 2.9
import QtQuick.Controls 2.3

ApplicationWindow {
	id: app
	visible: true
	width: uiCfg.gridSize * uiCfg.boardSize
	height: width + header.height + footer.height

	property bool isRunning: false
	property bool isBP: false
	property var order: []
	property int orderLength: order.length
	property int undoTimes: 10
	property int hintTimes: 10

	QtObject {
		id: uiCfg
		property int boardSize: 15
		property int gridSize: 32
		property string bgClr: "steelblue"
	}

	background: Rectangle {
		id: bg
		color: uiCfg.bgClr
	}

	header: MyHeader{id: myHeader}

	footer: MyFooter{
		id: myFooter
		startBE: !isRunning
		hintBE: isRunning && hintTimes > 0
		undoBE: isRunning && orderLength > 0 && undoTimes > 0
		onStart: go()
		onHint: getHint()
		onGiveUp: isRunning = false
		onUndo: undoLast()
	}

	GridView {
		id: board
		interactive: false
		visible: true
		width: uiCfg.gridSize * uiCfg.boardSize
		height: width
		cellWidth: uiCfg.gridSize
		cellHeight: cellWidth
		model: myModel
		delegate: myDelegate
	}

	ListModel {
		id: myModel
	}

	Component {
		id: myDelegate
		MyDelegate {
			width: uiCfg.gridSize
			color: uiCfg.bgClr
			leftV:		leftv
			rightV:		rightv
			upV:		upv
			downV:		downv
			centerV:	centerv
			chessmanV:	chessmanv
			buttonV:	isRunning
			onTriggered: genClr()

			function genClr() {
				changeUser()
				order.push(index)
				++orderLength
				if (isBP) {
					chessmanC = "black"
					user = "bp"
				} else {
					chessmanC = "white"
					user = "wp"
				}
				chessmanv = true
			}
		}
	}

	function aiPlay(r, l) {
		board.currentIndex = r * uiCfg.boardSize + l
		board.currentItem.click()
	}

	function go() {
		isRunning = true
		initBoard()
		aiPlay(parseInt(uiCfg.boardSize / 2), parseInt(uiCfg.boardSize / 2))
	}

	function getHint() {
		--hintTimes
		/// give hint
	}

	function undoLast() {
		--undoTimes
		var lastIdx = order.pop()
		myModel.setProperty(lastIdx, "user", "null")
		myModel.setProperty(lastIdx, "chessmanv", false)
		--orderLength
		changeUser()
	}

	function changeUser() {
		isBP = !isBP
	}

	function initBoard() {
		isBP = false
		order.splice(0, order.length)
		myModel.clear()
		for (var r = 0; r < uiCfg.boardSize; ++r) {
			for (var l = 0; l < uiCfg.boardSize; ++l) {
				myModel.append({"leftv": true, "rightv": true, "upv": true, "downv": true, "centerv": false, "chessmanv": false, "user": "null"})
				setModel(r, l)
			}
		}
	}

	function setModel(r, l) {
			if (r == 0) {
				myModel.setProperty(r * uiCfg.boardSize + l, "upv", false)
			} else if (r == uiCfg.boardSize - 1) {
				myModel.setProperty(r * uiCfg.boardSize + l, "downv", false)
			}

			if (l == 0) {
				myModel.setProperty(r * uiCfg.boardSize + l, "leftv", false)
			} else if (l == uiCfg.boardSize - 1) {
				myModel.setProperty(r * uiCfg.boardSize + l, "rightv", false)
			}
			var centerIdx = parseInt(uiCfg.boardSize / 2)
			if ( (l == r || (l + r) == (uiCfg.boardSize - 1))
			     && ((r == centerIdx) || (r == centerIdx - 4) || (r == centerIdx + 4)) ) {
				myModel.setProperty(r * uiCfg.boardSize + l, "centerv", true)
			}
	}

	Component.onCompleted: {
		initBoard()
	}


}
