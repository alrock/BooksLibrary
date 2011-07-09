import QtQuick 1.0
import app.bookslibrary 1.0

Rectangle {
	width: 640
	height: 480

	ListView {
		model: testModel
		anchors.fill: parent
		delegate: Row {
			Text {
				text: model.modelData.name
			}
		}
	}

	color: "red";
}
