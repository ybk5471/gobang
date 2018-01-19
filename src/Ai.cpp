#include <cmath>
#include <QDebug>
#include "../include/Ai.hpp"

Ai::Ai(QObject * parent) : QObject(parent) {}

Ai::~Ai() {}

int Ai::think(bool isAi, int deep) {

	int best_idx = -1;
	int best_score;
	best_idx = max(isAi, deep, best_score);
	qDebug() << best_idx;

	return best_idx;
}
void Ai::init(const QList<int> & situation) {

	m_situation.clear();
	m_row.clear();
	m_col.clear();
	m_slash.clear();
	m_backslash.clear();
	m_situation = situation;

	QList<int> singleList;

	//横
	for(int r = 0; r < m_boardSize; ++r) {
		singleList.clear();
		for (int l = 0; l < m_boardSize; ++l) {
			singleList.append(situation[r * m_boardSize + l]);
		}
		m_row.append(singleList);
	}
	//竖
	for(int l = 0; l < m_boardSize; ++l) {
		singleList.clear();
		for (int r = 0; r < m_boardSize; ++r) {
			singleList.append(situation[r * m_boardSize + l]);
		}
		m_col.append(singleList);
	}
	//斜
	for (int r =0, k = 0; k <= (m_boardSize - 1) * 2; ++k) {
		singleList.clear();
		for (int rt  = r; rt >= 0; --rt) {
			int l = k - rt;
			if (l > m_boardSize - 1) {
				break;
			}
			singleList.append(situation[rt * m_boardSize + l]);
		}
		if (singleList.count() >= 5) {
			m_slash.append(singleList);
		}
		if (r < m_boardSize - 1) {
			++r;
		}
	}
	//反斜
	for (int r = m_boardSize - 1, k = m_boardSize - 1; k >= -(m_boardSize - 1); --k) {
		singleList.clear();
		for (int rt  = r; rt <= m_boardSize - 1; ++rt) {
			int l = rt - k;
			if (l >  m_boardSize - 1) {
				break;
			}
			singleList.append(situation[rt * m_boardSize + l]);
		}
		if (singleList.count() >= 5) {
			m_backslash.append(singleList);
		}
		if (r > 0) {
			--r;
		}
	}
}
void Ai::refresh(int index, bool isAiValue) {
	int r = index / m_boardSize;
	int l = index % m_boardSize;
	setData(r, l, isAiValue);
}
// 自己得分最大化
int Ai::max(bool isAi, int deep, int & score) {
	deep = deep - 1;
	QList<int> max_best_idx_list;
	int bestIndex = -1;
	int maxAddedScore = -1;
	for (int r = 0; r < m_boardSize; ++r) {
		for (int l = 0; l < m_boardSize; ++l) {
			qDebug() << "scene: ";
			int sceneScore = calScore(isAi, r, l);
			if (!setData(isAi, r, l)) {
				continue;
			}
			qDebug() << "new: ";
			int newScore = calScore(isAi, r, l);
			int addedScore = newScore - sceneScore;
			qDebug() << r << ", " << l << "addedScore: "  << addedScore;
			if (maxAddedScore < addedScore) {
				max_best_idx_list.clear();
				maxAddedScore = addedScore;
				bestIndex = r * m_boardSize + l;
			} else if (maxAddedScore == addedScore) {
				max_best_idx_list.append(bestIndex);
				bestIndex = r * m_boardSize + l;
			}
			resetData(r, l);
		}
	}
	score = maxAddedScore;
	if (1 == deep) {
		return bestIndex;
	}

	int minAddedScore = 10000;
	int mscore;
	int min_score_index = -1;
	if (max_best_idx_list.count() != 0) {
		for (int i = 0; i < max_best_idx_list.count(); ++i) {
			int r = max_best_idx_list[i] / m_boardSize;
			int l = max_best_idx_list[i] % m_boardSize;
			setData(isAi, r, l);
			min(isAi, deep, mscore);
			if (mscore <= minAddedScore) {
				minAddedScore = mscore;
				min_score_index = max_best_idx_list[i];
			}
			resetData(r, l);
		}
	}
	setData(isAi, bestIndex / m_boardSize, bestIndex % m_boardSize);
	min(isAi, deep, mscore);
	resetData(bestIndex / m_boardSize, bestIndex % m_boardSize);
	if (mscore < minAddedScore) {
		return bestIndex;
	} else {
		return min_score_index;
	}

}
//对手得分最小化
int Ai::min(bool isAi, int deep, int & score) {
	deep = deep - 1;
	QList<int> min_best_idx_list;
	int bestIndex = -1;
	int minAddedScore = 10000;
	for (int r = 0; r < m_boardSize; ++r) {
		for (int l = 0; l < m_boardSize; ++l) {
			int sceneScore = calScore(!isAi, r, l);
			if (!setData(!isAi, r, l)) {
				continue;
			}
			int newScore = calScore(!isAi, r, l);
			int addedScore = newScore - sceneScore;
			if (addedScore < minAddedScore) {
				min_best_idx_list.clear();
				minAddedScore = addedScore;
				bestIndex = r * m_boardSize + l;
			} else if (addedScore == minAddedScore) {
				min_best_idx_list.append(bestIndex);
				bestIndex = r * m_boardSize + l;
			}
			resetData(r, l);
		}
	}
	score = minAddedScore;
	if (1 == deep) {
		return bestIndex;
	}

	int maxAddedscore = -1;
	int mscore;
	int max_score_index = -1;
	if (min_best_idx_list.count() != 0) {
		for (int i = 0; i < min_best_idx_list.count(); ++i) {
			int r = min_best_idx_list[i] / m_boardSize;
			int l = min_best_idx_list[i] % m_boardSize;
			setData(!isAi, r, l);
			max(isAi, deep, mscore);
			if (mscore >= maxAddedscore) {
				maxAddedscore = mscore;
				max_score_index = min_best_idx_list[i];
			}
			resetData(r, l);
		}
	}
	setData(!isAi, bestIndex / m_boardSize, bestIndex % m_boardSize);
	max(isAi, deep, mscore);
	resetData(bestIndex / m_boardSize, bestIndex % m_boardSize);
	if (mscore > maxAddedscore) {
		return bestIndex;
	} else {
		return max_score_index;
	}
}

int Ai::calScore(bool isAi, int r, int l) const {
	int score = 0;
	score += metaScore(isAi, m_row[r]);
	qDebug() << "m: " << score;
	score += metaScore(isAi, m_col[l]);
	qDebug() << "c: " << score;
	int sFirIdx = __toSFirIdx(r, l);
	if (sFirIdx >= 0 && sFirIdx < m_slash.count()) {
		score += metaScore(isAi, m_slash[sFirIdx]);
		qDebug() << "s: " << score;
	}
	int bFirIdx = __toBFirIdx(r, l);
	if (bFirIdx >= 0 && bFirIdx < m_backslash.count()) {
		score += metaScore(isAi, m_backslash[bFirIdx]);
		qDebug() << "b: " << score;
	}
	return score;
}

int Ai::metaScore(bool isAi, const QList<int> & mData) const {
	int score = 0;
	const int value = isAi ? m_aiValue : m_huValue;
	const int againstValue = isAi ? m_huValue : m_aiValue;

	int repeatValueCount = 0;
	int againstValueIdx = -1;
	long aresult = 0;
	long dresult = 0;

	for (int i = 0; i < mData.count() - 1; ++i) {
		if (mData.count() - againstValueIdx - 1 < 5) {
			break;
		}
		if (value == mData[i]) {
			repeatValueCount += 1;
			if (5 == repeatValueCount) {
				aresult += std::pow(10, repeatValueCount - 1);
				repeatValueCount = 0;
			}
			continue;
		}
		if (againstValue == mData[i]) {
			if (0 != repeatValueCount && i - againstValueIdx - 1 >=5) {
				dresult += std::pow(10, repeatValueCount - 1);
			}
			againstValueIdx = i;
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
	score = aresult + dresult / 10;
	return score;
}

bool Ai::setData(bool isAi, int r, int l) {
	qDebug() << r << ", " << l;
	int value = isAi ? m_aiValue : m_huValue;
	if (0 == m_situation[r * m_boardSize + l]) {
		m_situation[r * m_boardSize + l] = value;
	} else {
		qDebug() << r << ", " << l << ", is not error";
		return false;
	}
	if (0 == m_row[r][l]) {
		m_row[r][l] = value;
	} else {
		return false;
	}
	qDebug() << "old value: " << m_col[r][l];
	if (0 == m_col[l][r]) {
		m_col[l][r] = value;
	} else {
		return false;
	}
	qDebug() << "new value: " << m_col[r][l];
	int sfidx = __toSFirIdx(r, l);
	int scidx = __toSSecIdx(r, l);
	if (sfidx >= 0 && sfidx < m_slash.count()) {
		if (0 == m_slash[sfidx][scidx]) {
			m_slash[sfidx][scidx] = value;
		} else {
			return false;
		}
	}
	int bsidx = __toBFirIdx(r, l);
	int bcidx = __toBSecIdx(r, l);
	if (bsidx >= 0 && bsidx < m_backslash.count()) {
		if (0 == m_backslash[bsidx][bcidx]) {
			m_backslash[bsidx][bcidx] = value;
		} else {
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
		return false;
	}
	if (m_col[l][r] != 0) {
		m_col[l][r] = 0;
	} else {
		return false;
	}
	int sfidx = __toSFirIdx(r, l);
	int scidx = __toSSecIdx(r, l);
	if (sfidx >= 0 && sfidx < m_slash.count()) {
		if (m_slash[sfidx][scidx] != 0) {
			m_slash[sfidx][scidx] = 0;
		} else {
			return false;
		}
	}
	int bsidx = __toBFirIdx(r, l);
	int bcidx = __toBSecIdx(r, l);
	if (bsidx >= 0 && bsidx < m_backslash.count()) {
		if (m_backslash[bsidx][bcidx] != 0) {
			m_backslash[bsidx][bcidx] = 0;
		} else {
			return false;
		}
	}
	return true;
}
