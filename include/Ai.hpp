#pragma once

#include <QObject>
#include <QList>

class Ai : public QObject {
	Q_OBJECT
public:
	Q_PROPERTY(int boardSize MEMBER m_boardSize);

	Ai(QObject * parent = 0);
	~Ai();

	Q_INVOKABLE int think(bool isAi, int deep, const QList<int> & situation);
private:
	int max(bool isAi, int deep, int & score);
	int min(bool isAi, int deep, int & score);
	int calScore(bool isAi, const QList<QList<int>> & data) const;
	int metaScore(bool isAi, const QList<int> & mData) const;
	bool setData(bool isAi, int r, int l);
	bool resetData(int r, int l);
private:
	int m_boardSize;
	QList<int> m_situation;
	QList<QList<int>> m_row;
	QList<QList<int>> m_col;
	QList<QList<int>> m_slash;
	QList<QList<int>> m_backslash;
};
