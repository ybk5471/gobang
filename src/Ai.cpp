#include <cmath>
#include <QDebug>
#include "../include/Ai.hpp"

Ai::Ai(QObject * parent) : QObject(parent) {}

Ai::~Ai() {}

int Ai::think(bool isAi, int deep, const QList<int> & situation) {
	m_situation = situation;

	QList<int> sr;
	QList<int> sc;
	QList<int> ss;
	QList<int> bs;

	int best_idx = -1;
	//横
	for(int r = 0; r < m_boardSize; ++r) {
		sr.clear();

		for (int l = 0; l < m_boardSize; ++l) {
			sr.append(situation[r * m_boardSize + l]);
		}
		m_row.append(sr);
	}
	//竖
	for(int l = 0; l < m_boardSize; ++l) {
		sc.clear();
		for (int r = 0; r < m_boardSize; ++r) {
			sc.append(situation[r * m_boardSize + l]);
		}
		m_col.append(sc);
	}
	//斜
	for (int r =0, k = 0; k <= (m_boardSize - 1) * 2; ++k) {
		ss.clear();
		for (int rt  = r; rt >= 0; --rt) {
			int l = k - rt;
			if (l > m_boardSize - 1) {
				break;
			}
			ss.append(situation[rt * m_boardSize + l]);
		}
		if (ss.count() >= 5) {
			m_slash.append(ss);
		}
		if (r < m_boardSize - 1) {
			++r;
		}
	}
// 	for (int r = 0; r < m_boardSize; ++r) {
// 		ss.clear();
// 		int tr = r;
// 		for (int ll = 0; tr >= 0; ++ll, --tr) {
// 			ss.append(situation[tr * m_boardSize + ll]);
// 		}
// 		if (ss.count() >= 5) {
// 			m_slash.append(ss);
// 		}
// 		ss.clear();
// 		tr = r;
// 		for (int rl = m_boardSize - 1; tr < m_boardSize; --rl, ++tr) {
// 			if (rl + tr == m_boardSize) {
// 				continue;
// 			}
// 			ss.append(situation[tr * m_boardSize + rl]);
// 		}
// 		if (ss.count() >= 5) {
// 			m_slash.append(ss);
// 		}
// 	}
	//反斜
	for (int r = m_boardSize - 1, k = m_boardSize - 1; k >= -(m_boardSize - 1); --k) {
		bs.clear();
		for (int rt  = r; rt <= m_boardSize - 1; ++rt) {
			int l = rt - k;
			if (l >  m_boardSize - 1) {
				break;
			}
			bs.append(situation[rt * m_boardSize + l]);
		}
		if (bs.count() >= 5) {
			m_backslash.append(bs);
		}
		if (r > 0) {
			--r;
		}
	}
// 	for (int l = 0; l < m_boardSize; ++l) {
// 		bs.clear();
// 		int tl = l;
// 		for (int lr = m_boardSize - 1; tl > 0; --lr, --tl) {
// 			bs.append(situation[lr* m_boardSize + tl]);
// 		}
// 		if (bs.count() >= 5) {
// 			m_backslash.append(bs);
// 		}
// 		bs.clear();
// 		tl = l;
// 		for (int rr = 0; tl < m_boardSize; ++rr, ++tl) {
// 			if (rr == tl) {
// 				continue;
// 			}
// 			bs.append(situation[rr * m_boardSize + tl]);
// 		}
// 		if (bs.count() >= 5) {
// 			m_backslash.append(bs);
// 		}
// 	}

	int best_score;

	best_idx = max(isAi, deep, best_score);

	return best_idx;
}
// 自己得分最大化
int Ai::max(bool isAi, int deep, int & score) {
	deep = deep - 1;
	QList<int> max_best_idx_list;
	int best_index = -1;
	int best_score = -1;
	for (int r = 0; r < m_boardSize; ++r) {
		for (int l = 0; l < m_boardSize; ++l) {
			if (!setData(isAi, r, l)) {
				continue;
			}
			int tmp = -1;
			tmp = calScore(isAi, m_row) + calScore(isAi, m_col) + calScore(isAi, m_slash) + calScore(isAi, m_backslash);
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

	if (1 == deep) {
		score = best_score;
		qDebug() << "best score: " << score;
		return best_index;
	}

	int min_score = 10000;
	int mscore;
	int min_score_index = -1;
	if (max_best_idx_list.count() != 0) {
		for (int i = 0; i < max_best_idx_list.count(); ++i) {
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
//对手得分最小化
int Ai::min(bool isAi, int deep, int & score) {
	deep = deep - 1;
	QList<int> min_best_idx_list;
	int best_index = -1;
	int best_score = 10000;
	for (int r = 0; r < m_boardSize; ++r) {
		for (int l = 0; l < m_boardSize; ++l) {
			if (!setData(!isAi, r, l)) {
				continue;
			}
			int tmp = -1;
			tmp = calScore(!isAi, m_row) + calScore(!isAi, m_col) + calScore(!isAi, m_slash) + calScore(!isAi, m_backslash);
			if (tmp < best_score) {
				min_best_idx_list.clear();
				best_score = tmp;
				best_index = r * m_boardSize + l;
			} else if (tmp == best_score) {
				min_best_idx_list.append(best_index);
				best_index = r * m_boardSize + l;
			}
			resetData(r, l);
		}
	}

	if (1 == deep) {
		score = best_score;
		return best_index;
	}

	int max_score = -1;
	int mscore;
	int max_score_index = -1;
	if (min_best_idx_list.count() != 0) {
		for (int i = 0; i < min_best_idx_list.count(); ++i) {
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

int Ai::calScore(bool isAi, const QList<QList<int>> & data) const {
	int score = 0;
	for (int i = 0; i < data.count(); ++i) {
		const auto & d = data[i];
		score += metaScore(isAi, d);
	}
	return score;
}

int Ai::metaScore(bool isAi, const QList<int> & mData) const {
	int score = 0;
	const int value = isAi ? 1 : 2;
	const int againstValue = isAi ? 2 : 1;

	int repeatValueCount = 0;
	int againstValueIdx = -1;
	long aresult = 0;
	long dresult = 0;

// 	int a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, d1 = 0, d2 = 0, d3 = 0, d4 = 0;

	for (int i = 0; i < mData.count() - 1; ) {
		if (mData.count() - againstValueIdx - 1 < 5) {
			break;
		}
		if (value == mData[i]) {
			repeatValueCount += 1;
			++i;
			if (5 == repeatValueCount) {
				aresult += std::pow(10, repeatValueCount - 1);
				repeatValueCount = 0;
			}
			continue;
		}
		if (againstValue == mData[i]) {
			if (0 != repeatValueCount && i - againstValueIdx - 1 >=5) {
// 				qDebug() << "arepeatValueCount: " << repeatValueCount;
				dresult += std::pow(10, repeatValueCount - 1);
			}
			againstValueIdx = i;
			++i;
			repeatValueCount = 0;
			continue;
		}
		if (0 == mData[i]) {
			int secAgainstValueIdx = mData.count();
			for (int k = 1; k < mData.count() - i - 1; ++k) {
				if (againstValue == mData[i + k]) {
					secAgainstValueIdx = i + k;
				}
			}
			if (0 != repeatValueCount && secAgainstValueIdx - againstValueIdx - 1 >= 5) {
				if (i - againstValueIdx - 1  == repeatValueCount) {
					dresult += std::pow(10, repeatValueCount - 1);
				} else {
					aresult += std::pow(10, repeatValueCount - 1);
				}
			}
			if (repeatValueCount > 3) {
				qDebug() << "zrepeatValueCount: " << repeatValueCount;
			}
			++i;
			repeatValueCount = 0;
			continue;
		}
// 		if (0 == mData[i] && i != mData.count() - 1 && 0 != mData[i + 1]) {
// 			int secAgainstValueIdx = mData.count();
// 			bool extra = false;
// 			int extraScore = 0;
// 			for (int k = 1; k < mData.count() - i - 1; ++k) {
// 				int rv = 0;
// 				if (value == mData[i + k]) {
// 					++rv;
// 				} else if (againstValue == mData[i + k]) {
// 					secAgainstValueIdx = i + k;
// 					break;
// 				} else if (i + k == mData.count() - 1) {
// 					break;
// 				} else if (0 == mData[i + k]) {
// 					extraScore = std::pow(10, rv - 1);
// 					break;
// 				}
// 			}
// 			if (0 != repeatValueCount && secAgainstValueIdx - againstValueIdx - 1 >= 5) {
// 				if (i - againstValueIdx - 1  == repeatValueCount) {
// 					dresult = std::pow(10, repeatValueCount - 1);
// 				} else {
// 					aresult = std::pow(10, repeatValueCount - 1);
// 					extra = true;
// 				}
// 			}
// 			if (extra && extraScore) {}
// 			++i;
// 			repeatValueCount = 0;
// 			continue;
// 		}
	}

// 	qDebug() << "aresult: " << aresult;
// 	qDebug() << "dresult: " << dresult;

// 	a5 = aresult % 100000;
// 	a4 = aresult % 10000;
// 	a3 = aresult % 1000;
// 	a2 = aresult % 100;
// 	a1 = aresult % 10;
// 	d4 = dresult % 10000;
// 	d3 = dresult % 1000;
// 	d2 = dresult % 100;
// 	d1 = dresult % 10;

// 	score = a1 * 1 + a2 * 2 + a3 * 4 + a4 * 500 + a5 * 1000 + d1 * 0 + d2 * 1 + d3 * 2 + d4 * 4;
	score = aresult + dresult / 10;
	return score;
}

bool Ai::setData(bool isAi, int r, int l) {
	int value = isAi ? 1 : 2;
	if (0 == m_situation[r * m_boardSize + l]) {
		m_situation[r * m_boardSize + l] = value;
	} else {
		qDebug() << "is not zero: " << r << ", col: " << l << "svalue: " << m_situation[r * m_boardSize + l];
		return false;
	}
	if (0 == m_row[r][l]) {
		m_row[r][l] = value;
	} else {
		qDebug() << "set row data error: " << r << ", col: " << l << ", value: " << m_row[r][l] << "svalue: " << m_situation[r * m_boardSize + l];
		return false;
	}
	if (0 == m_col[l][r]) {
		m_col[l][r] = value;
	} else {
		qDebug() << "set row data error: " << r << ", col: " << l << ", value: " << m_col[l][r] << "svalue: " << m_situation[r * m_boardSize + l];
		return false;
	}
	int sfidx = r + l - 4;
	int scidx = (sfidx + 4 > m_boardSize - 1) ? (l - (sfidx + 4 - (m_boardSize - 1))) : l;
	if (sfidx >= 0 && sfidx < m_slash.count()) {
		if (0 == m_slash[sfidx][scidx]) {
			m_slash[sfidx][scidx] = value;
		} else {
			qDebug() << "set slash data error: " << r << ", col: " << l << ", sfidx: " << sfidx << ", scidx: " << scidx;
			return false;
		}
	}
	int bsidx = -(r - l - m_boardSize + 1) - 4;
	int bcidx = (bsidx + 4 > m_boardSize - 1) ? (l - (bsidx + 4 - (m_boardSize - 1))) : l;
	if (bsidx >= 0 && bsidx < m_backslash.count()) {
		if (0 == m_backslash[bsidx][bcidx]) {
			m_backslash[bsidx][bcidx] = value;
		} else {
			qDebug() << "set backslash data error: " << r << ", col: " << l << ", bsidx: " << bsidx << ", bcidx: " << bcidx;
			return false;
		}
	}
	return true;
}

bool Ai::resetData(int r, int l) {
	if (m_situation[r * m_boardSize + l] != 0) {
		m_situation[r * m_boardSize + l] = 0;
	} else {
		return false;
	}
	if (m_row[r][l] != 0) {
		m_row[r][l] = 0;
	} else {
		qDebug() << "reset row data error: row!!!!!";
		return false;
	}
	if (m_col[l][r] != 0) {
		m_col[l][r] = 0;
	} else {
		qDebug() << "reset row data error!!!!!";
		return false;
	}
	int sfidx = r + l - 4;
	int scidx = (sfidx + 4 > m_boardSize - 1) ? (l - (sfidx + 4 - (m_boardSize -1))) : l;
	if (sfidx >= 0 && sfidx < m_slash.count()) {
		if (m_slash[sfidx][scidx] != 0) {
			m_slash[sfidx][scidx] = 0;
		} else {
			qDebug() << "reset slash data error!!!!!";
			return false;
		}
	}
	int bsidx = -(r - l - m_boardSize + 1) - 4;
	int bcidx = (bsidx + 4 > m_boardSize - 1) ? (l - (bsidx + 4 - (m_boardSize - 1))) : l;
	if (bsidx >= 0 && bsidx < m_backslash.count()) {
		if (m_backslash[bsidx][bcidx] != 0) {
			m_backslash[bsidx][bcidx] = 0;
		} else {
			qDebug() << "reset backslash data error!!!!!";
			return false;
		}
	}
	return true;
}
