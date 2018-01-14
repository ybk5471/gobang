#include "Ai.hpp"

Ai::Ai(QObject * parent) : QObject(parent {}

Ai::~Ai() {}

int Ai::think(bool isAi, int deep, const QList<int> & situation) {

	m_situation = situation;

	QList<int> sr;
	QList<int> sc;
	QList<int> ss;
	QList<int> sb;

	int best_idx = -1;

	for(int r = 0; r < m_boardSize - 1; ++r) {
		sr.clear();
		for (int l = 0; l < m_boardSize - 1; ++l) {
			sr.append(situation[r * m_boardSize + l]);
		}
		m_row.append(sr);
	}

	for(int l = 0; l < m_boardSize - 1; ++l) {
		sc.clear();
		for (int r = 0; r < m_boardSize - 1; ++r) {
			sc.append(situation[r * m_boardSize + l]);
		}
		m_col.append(sc);
	}

	for (int r = 0; r < m_boardSize - 1; ++r) {
		ss.clear();
		int tr = r;
		for (int ll = 0; tr >= 0; ++ll, --tr) {
			ss.append(situation[tr * m_boardSize + ll]);
		}
		if (ss.count >= 5) {
			m_slash.append(ss);
		}
		ss.clear();
		tr = r;
		for (int rl = m_boardSize - 1; tr < m_boardSize; --rl, ++tr) {
			ifmax (rl + tr == m_boardSize - 1) {
				continue;
			}
			ss.append(situation[tr * m_boardSize + rl]);
		}
		if (ss.count >= 5) {
			m_slash.append(ss);
		}
	}

	for (int l = 0; l < m_boardSize - 1; ++l) {
		bs.clear();
		int tl = l;
		for (int lr = m_boardSize - 1; tl > 0; --lr, --tl) {
			bs.append(situation[lr* m_boardSize + tl]);
		}
		if (bs.count >= 5) {
			m_backslash.append(bs);
		}
		bs.clear();
		tl = l;
		for (int rr = 0; tl < m_boardSize; ++rr, ++tl) {
			if (rr == tl) {
				continue;
			}
			bs.append(situation[rr * m_boardSize + tl]);
		}
		if (bs.count >= 5) {
			m_backslash.append(bs);
		}
	}

	int best_score;

	best_idx = max(isAi, deep, best_score);

	return best_idx;
}

int Ai::max(bool isAi, int deep, int & score) {
	deep = deep - 1;
	QList<int> max_best_idx_list;
	int best_index = -1
	int best_score = -1;
	for (int r = 0; r < m_boardSize; ++r) {
		for (int l = 0; l < m_boardSize; ++l) {
			if (!setData(isAi, r, l)) {
				resetData(r, l);
				continue;
			}
			int tmp = -1;
			tmp = calScore(m_row) + calScore(m_col) + calScore(m_slash) + calScore(m_backslash);
			if (best_score < tmp) {
				max_best_idx_list.clear();
				best_score = tmp;
				best_index = r * m_boardSize + l;
			} else if (best_score == tmp) {
				max_best_idx_list.append(best_index);
				best_index = r * m_boardSize + l;
			}
			resetData(r, l);
		}
	}

	if (deep == 1) {
		score = best_score;
		return best_index;
	}

	int min_score = 10000;
	int mscore;
	int min_score_index;
	if (max_best_idx_list.count() != 0) {
		for (var i = 0; i < max_best_idx_list.count(); ++i) {
			int r = max_best_idx_list[i] / m_boardSize;
			int l = max_best_idx_list[i] % m_boardSize;
			setData(isAi, r, l);
			min(isAi, deep, mscore);
			if (mscore <= min_score) {
				min_score = mscore;
				min_score_index = max_best_idx_list[i];
			}
			resetData(r, l);
		}
	}
	setData(isAi, best_index / m_boardSize, best_index % m_boardSize);
	min(isAi, deep, mscore);
	score = best_score;
	if (mscore < min_score) {
		return best_index;
	} else {
		return min_score_index;
	}

}

int Ai::min(bool isAi, int deep, int & score) {
	deep = deep - 1;
	QList<int> min_best_idx_list;
	int best_index = -1;
	int best_score = 10000;
	for (int r = 0; r < m_boardSize; ++r) {
		for (int l = 0; l < m_boardSize; ++l) {
			if (!setData(!isAi, r, l)) {
				resetData(r, l);
				continue;
			}
			int tmp = -1;
			tmp = calScore(m_row) + calScore(m_col) + calScore(m_slash) + calScore(m_backslash);
			if (tmp < best_score) {
				min_best_idx_list.clear();
				best_score = tmp;
				best_index = r * m_boardSize + l;
			} else if (tmp = best_score) {
				min_best_idx_list.push(best_index);
				best_index = r * m_boardSize + l;
			}
			resetData(r, l);
		}
	}

	if (deep == 1) {
		score = best_score;
		return best_index;
	}

	int max_score = -1;
	int mscore;
	int max_score_index;
	if (min_best_idx_list.count() != 0) {
		for (var i = 0; i < min_best_idx_list.count(); ++i) {
			int r = min_best_idx_list[i] / m_boardSize;
			int l = min_best_idx_list[i] % m_boardSize;
			setData(!isAi, r, l);
			max(isAi, deep, mscore);
			if (mscore >= max_score) {
				max_score = mscore;
				max_score_index = min_best_idx_list[i];
			}
			resetData(r, l);
		}
	}
	setData(!isAi, best_index / m_boardSize, best_index % m_boardSize);
	min(isAi, deep, mscore);
	score = best_score;
	if (mscore < max_score) {
		return best_index;
	} else {
		return max_score_index;
	}
}

int calScore(bool isAi, const QList<QList<int>> & data) const {
	int value = isAi ? 1 : 2;
	for (int i = 0; i < data.count(); ++i) {

	}
}

bool Ai::setData(bool isAi, int r, int l) {
	int value = isAi ? 1 : 2;
	if (m_situation[r][l] == 0) {
		m_situation[r][l] = value;
	} else {
		return false;
	}
	if (m_row[r][l] == 0) {
		m_row[r][l] = value;
	} else {
		qDebug() << "set row data error!!!!!";
		return false;
	}
	if (m_col[l][r] == 0) {
		m_col[l][r] = value;
	} else {
		qDebug() << "set row data error!!!!!";
		return false;
	}
	if (m_slash[r + l - 5][l] == 0) {
		m_slash[r + l - 5][l] = value;
	} else {
		qDebug() << "set slash data error!!!!!";
		return false;
	}
	if (m_backslash[r + l - 5][r] == 0) {
		m_backslash[r + l - 5][r] = value;
	} else {
		qDebug() << "set backslash data error!!!!!"
		return false;
	}
	return true;
}

bool Ai::resetData(int r, int l) {
	if (m_situation[r][l] != 0) {
		m_situation[r][l] = 0;
	} else {
		return false;
	}
	if (m_row[r][l] != 0) {
		m_row[r][l] = 0;
	} else {
		qDebug() << "reset row data error!!!!!";
		return false;
	}
	if (m_col[l][r] != 0) {
		m_col[l][r] = 0;
	} else {
		qDebug() << "reset row data error!!!!!";
		return false;
	}
	if (m_slash[r + l - 5][l] != 0) {
		m_slash[r + l - 5][l] = 0;
	} else {
		qDebug() << "reset slash data error!!!!!";
		return false;
	}
	if (m_backslash[r + l - 5][r] != 0) {
		m_backslash[r + l - 5][r] = 0;
	} else {
		qDebug() << "reset backslash data error!!!!!"
		return false;
	}
	return true;
}
