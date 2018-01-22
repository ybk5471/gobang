#pragma once

#include <QObject>
#include <QList>

class Ai : public QObject {
	Q_OBJECT
public:
	Q_PROPERTY(int boardSize MEMBER m_boardSize);
	Q_PROPERTY(int aiValue MEMBER m_aiValue);
	Q_PROPERTY(int huValue MEMBER m_huValue);

	Ai(QObject * parent = 0);
	~Ai();

	Q_INVOKABLE int think(bool isAi, int deep);
	Q_INVOKABLE void init(const QList<int> & situation);
	Q_INVOKABLE void refresh(bool isAiValue, int index);

private:
	int max(bool isAi, int deep, int & score);
	int min(bool isAi, int deep, int & score);
	int calScore(bool isAi, int r, int l) const;
	int metaScore(bool isAi, const QList<int> & mData) const;
	bool setData(bool isAi, int r, int l);
	bool resetData(int r, int l);

	int __toSFirIdx(int r, int l) const {
		return r + l - 4;
	}

	int __toSSecIdx(int r, int l)  {
		int sFirIdx = __toSFirIdx(r, l);
		return (sFirIdx + 4 > m_boardSize - 1) ? (l - (sFirIdx + 4 - (m_boardSize - 1))) : l;
	}

	int __toBFirIdx(int r, int l) const {
		return -(r - l - m_boardSize + 1) - 4;
	}

	int __toBSecIdx(int r, int l) const {
		int bFirIdx = __toBFirIdx(r, l);
		return (bFirIdx + 4 > m_boardSize - 1) ? (l - (bFirIdx + 4 - (m_boardSize - 1))) : l;
	}

private:
	int m_boardSize;
	int m_aiValue;
	int m_huValue;
	QList<int> m_situation;
	QList<QList<int>> m_row;
	QList<QList<int>> m_col;
	QList<QList<int>> m_slash;
	QList<QList<int>> m_backslash;
};
