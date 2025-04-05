#include <bits/stdc++.h>
#include "common.h"

struct TestGen : BaseTestGen {
	int64_t maxN;
	int64_t maxM;
	int64_t maxSum;
	int64_t maxQ;
	std::optional<int64_t> maxSetSize;
	std::optional<int64_t> maxNodeSets;

	TestGen (
		const int64_t count, 
		const int64_t maxN, 
		const int64_t maxM, 
		const int64_t maxSum,
		const int64_t maxQ,
		const std::optional<int64_t> maxSetSize,
		const std::optional<int64_t> maxNodeSets
	) : BaseTestGen(count), maxN(maxN), maxM(maxM), maxSum(maxSum), maxQ(maxQ), maxSetSize(maxSetSize), maxNodeSets(maxNodeSets) {}

	void finalize(
		std::ostream& out,
		const int64_t n,
		const std::vector<std::vector<int64_t>> &sets, 
		const std::vector<std::pair<int64_t, int64_t>> &queries
	) {
		{
			std::vector<int64_t> count(n, 0);
			assert(n <= maxN);
			assert(sets.size() <= maxM);
			int64_t sum = 0;
			for (const auto &it : sets) {
				sum += it.size(); 
				assert(it.size() >= 2);
				if (maxSetSize) {
					assert(it.size() <= maxSetSize.value());
				}
				for (const auto itt : it) {
					assert(0 <= itt && itt < n);
				}
			}
			for (const auto it : count) {
				if (maxNodeSets) {
					assert(it <= maxNodeSets.value());
				}
			}
			assert(sum <= maxSum);
			assert(queries.size() <= maxQ);
			for (const auto &it : queries) {
				assert(0 <= it.first < n);
				assert(0 <= it.second < n);
				assert(it.first != it.second);
			}
		}

		out << n << " " << sets.size() << "\n";
		for (const auto &st : sets) {
			out << st.size() << " ";
			for (const auto it : st) {
				out << it + 1 << " ";
			}
			out << "\n";
		}
		
		out << queries.size() << std::endl;
		for (const auto &it : queries) {
			out << it.first + 1 << " " << it.second + 1 << std::endl;
		}
	}
};


struct SimpleTestGen : TestGen {
	bool sameSizeOrPart;

	SimpleTestGen(
		const int64_t count, 
		const int64_t maxN, 
		const int64_t maxM, 
		const int64_t maxSum,
		const int64_t maxQ,
		const std::optional<int64_t> maxSetSize = std::nullopt,
		const std::optional<int64_t> maxNodeSets = std::nullopt,
		const bool sameSizeOrPart = false
	) : TestGen(count, maxN, maxM, maxSum, maxQ, maxSetSize, maxNodeSets), sameSizeOrPart(sameSizeOrPart) { }

	void generate(std::ostream& out) {
		const auto n = rnd.wnext(1ll, (long long)maxN, 1000);
		const auto m = rnd.wnext(1ll, std::min((long long)maxM, maxSum / 2ll), 1000);
		const auto setSum = rnd.wnext(m * 2ll, std::min(n * m, (long long)(maxSum)), 1000ul);

		std::vector<int64_t> setSizes;
		if (sameSizeOrPart) {
			setSizes = rnd.partition<int64_t>(m, setSum, 2);
		} else {
			setSizes.resize(m);
			for (int64_t i = 0; i < setSum; i ++) {
				setSizes[i % m] += 1;
			}
		}
		std::vector<std::vector<int64_t>> sets;
		std::vector<int64_t> goodSets;

		for (const auto setSize : setSizes) {
			const auto newSize = std::min({(int64_t)n, (int64_t)setSize, maxSetSize.value_or(setSize)});
			const auto setElements = rnd.distinct<int64_t>(newSize, n);
			sets.push_back(setElements);

			if (sets.back().size() >= 2) {
				goodSets.push_back(sets.size() - 1);
			}
		}

		const auto q = rnd.wnext(1ll, (long long)maxQ, 1000);
		std::vector<std::pair<int64_t, int64_t>> queries;
		for (int64_t i = 0; i < q; i ++) {
			if (rnd.next(2) == 0 && !goodSets.empty()) {
				const auto id = goodSets[rnd.next(goodSets.size())];
				const auto pair = rnd.distinct(2, sets[id].size());
				queries.emplace_back(sets[id][pair[0]], sets[id][pair[1]]);
			} else {
				const auto pair = rnd.distinct(2, n);
				queries.emplace_back(pair[0], pair[1]);
			}
		}

		finalize(out, n, sets, queries);
	}
};

struct BigSetsTestGen : TestGen {
	enum class Mode {
		BIG,
		SQRT,
		EQUAL,
		SMALL,
		FIXED
	};

	Mode mode;
	std::optional<int64_t> fixedValue;

	BigSetsTestGen(
		const int64_t count, 
		const int64_t maxN, 
		const int64_t maxM, 
		const int64_t maxSum,
		const int64_t maxQ,
		const std::optional<int64_t> maxSetSize = std::nullopt,
		const std::optional<int64_t> maxNodeSets = std::nullopt,
		const Mode mode = Mode::SQRT,
		const std::optional<int64_t> fixedValue = std::nullopt
	) : TestGen(count, maxN, maxM, maxSum, maxQ, maxSetSize, maxNodeSets), mode(mode), fixedValue(fixedValue) {}

	void generate(std::ostream& out) {
		const auto n = rnd.wnext(1ll, (long long)maxN, 1000ul);
		const auto m = rnd.wnext(1ll, std::min((long long)maxM, maxSum / 2ll), 1000);

		const int64_t heavyPeople = [&] () {
			int64_t inter;
			switch (mode) {
				case Mode::BIG: inter = rnd.next(1ll, n); break;
				case Mode::SQRT: inter = sqrt(n) / 16 * rnd.next(1, 4); break;
				case Mode::EQUAL: inter = maxSum / maxM; break;
				case Mode::SMALL: inter = 5 + rnd.next(5) * 10; break;
				case Mode::FIXED: inter = fixedValue.value(); break;
			}
			inter = std::max(inter, (int64_t)4);
			inter = std::min(inter, (int64_t)n);
			if (maxSetSize) {
				return std::min((int64_t)maxSetSize.value(), inter);
			} else {
				return inter;
			}
		}();

		const int64_t lightPeople = rnd.next((long long)0, std::min((long long)5, (long long)maxSetSize.value_or(n) - heavyPeople));

		const auto coolGroup = rnd.distinct(heavyPeople + lightPeople, n);

		std::vector<int64_t> heavyGroup;
		for (int64_t i = 0; i < heavyPeople; i ++) { heavyGroup.push_back(coolGroup[i]); }
		std::vector<int64_t> lightGroup;
		for (int64_t i = 0; i < lightPeople; i ++) { lightGroup.push_back(coolGroup[i + heavyPeople]); }

		std::vector<std::vector<int64_t>> sets;
		std::vector<std::pair<int64_t, int64_t>> queries;

		int64_t currentSum = 0;

		while (sets.size() < maxM && currentSum < maxSum) {
			std::vector<int64_t> newSet;
			const auto heavySetIndexes = rnd.distinct<int>(heavyPeople - 2, heavyPeople);
			for (const auto it : heavySetIndexes) { newSet.push_back(heavyGroup[it]); }
			for (const auto it : lightGroup) {
				if (rnd.next(2)) {
					newSet.push_back(it);
				}
			}

			if (currentSum + newSet.size() > maxSum) {
				const auto newSetSize = maxSum - currentSum;
				if (newSetSize >= 2) {
					newSet.resize(newSetSize);
					sets.push_back(newSet);
				}
				break;
			} else {
				currentSum += newSet.size();
				sets.push_back(newSet);
			}
		}

		if ((long long)coolGroup.size() * (long long)coolGroup.size() > maxQ) {
			while (queries.size() < maxQ) {
				const auto pair = rnd.distinct(2, coolGroup.size());
				const auto a = coolGroup[0];
				const auto b = coolGroup[1];
				queries.emplace_back(coolGroup[pair[0]], coolGroup[pair[1]]);
			}
		} else {
			while (queries.size() < maxQ) {
				for (const auto it : coolGroup) {
					for (const auto itt : coolGroup) if (it != itt) {
						queries.emplace_back(it, itt);
					}
				}
			}
			shuffle(queries.begin(), queries.end());
			while (queries.size() > maxQ) { queries.pop_back(); }
		}

		finalize(out, n, sets, queries);
	}
};

struct Kill2TestGen : TestGen {
	Kill2TestGen(
		const int64_t count, 
		const int64_t maxN, 
		const int64_t maxM, 
		const int64_t maxSum,
		const int64_t maxQ,
		const std::optional<int64_t> maxSetSize = std::nullopt,
		const std::optional<int64_t> maxNodeSets = std::nullopt
	) : TestGen(count, maxN, maxM, maxSum, maxQ, maxSetSize, maxNodeSets) {}

	void generate(std::ostream& out) {
		const auto n = rnd.wnext(1ll, (long long)maxN, 1000ul);
		const auto m = rnd.wnext(1ll, std::min((long long)maxM, maxSum / 2ll), 1000);

		std::vector<std::vector<int64_t>> sets;
		std::vector<std::pair<int64_t, int64_t>> queries;

		long long leftPartSize = std::min((long long)maxSum / 4, n);
		if (maxSetSize) {
			leftPartSize = std::min(leftPartSize, (long long)maxSetSize.value());
		}
		auto leftPart = rnd.distinct<int64_t>(leftPartSize, n);
		const auto a = leftPart[0], b = leftPart[1];

		int64_t currentSum = leftPart.size();
		sets.push_back(leftPart);
		while (sets.size() < maxM) {
			std::vector<int64_t> newSet = {a, b};
			if (currentSum + newSet.size() > maxSum) {
				break;
			} else {
				currentSum += newSet.size();
				sets.push_back(newSet);
			}
		}

		while (queries.size() < maxQ) {
			if (rnd.next(2)) {
				queries.push_back({a, b});
			} else {
				queries.push_back({b, a});
			}
		}

		finalize(out, n, sets, queries);
	}
};

struct BoundedNodesTestGen : TestGen {
	enum class ModeNodes {
		SMALL,
		BIG
	};

	ModeNodes modeNodes;
	int64_t weightSide;

	BoundedNodesTestGen(
		const int64_t count, 
		const int64_t maxN, 
		const int64_t maxM, 
		const int64_t maxSum,
		const int64_t maxQ,
		const std::optional<int64_t> maxSetSize = std::nullopt,
		const std::optional<int64_t> maxNodeSets = std::nullopt,
		const ModeNodes modeNodes = ModeNodes::SMALL,
		const int64_t weightSide = 1
	) : TestGen(count, maxN, maxM, maxSum, maxQ, maxSetSize, maxNodeSets), modeNodes(modeNodes), weightSide(weightSide) {}

	void generate(std::ostream& out) {
		const auto n = rnd.wnext(1ll, (long long)maxN, 1000ul);
		const auto m = rnd.wnext(1ll, std::min((long long)maxM, maxSum / 2ll), 1000);
		const auto setSum = rnd.wnext(m * 2ll, std::min(n * m, (long long)(maxSum)), 1000ul);

		std::vector<int64_t> count(n, maxNodeSets.value_or(n));
		std::set<std::pair<int64_t, int64_t>> set;

		for (int64_t i = 0; i < n; i ++) {
			set.insert({count[i], i});
		}

		std::vector<int64_t> setSizes;
		if (rnd.next(2)) {
			setSizes = rnd.partition<int64_t>(m, setSum, 2);
		} else {
			setSizes.resize(m);
			for (int64_t i = 0; i < setSum; i ++) {
				setSizes[i % m] += 1;
			}
		}

		std::vector<std::vector<int64_t>> sets;
		std::vector<std::pair<int64_t, int64_t>> queries;

		for (const auto it : setSizes) {
			std::vector<int64_t> current = {};
			while (current.size() < it) {
				auto curr = (modeNodes == ModeNodes::SMALL ? set.begin() : set.end());
				if (modeNodes == ModeNodes::BIG) { curr --; }
				const auto node = curr->second;
				set.erase(curr);
				if (count[node] <= 0) { continue; }
				current.push_back(node);
			}
			sets.push_back(current);
			for (const auto itt : current) {
				count[itt] --;
				set.insert({count[itt], itt});
			}
		}

		std::vector<std::pair<int64_t, int64_t>> all;
		for (int64_t i = 0; i < n; i ++) {
			all.push_back({count[i], i});
		}
		std::sort(all.begin(), all.end());

		while (queries.size() < maxQ) {
			const auto a_ = rnd.wnext(0ll, (long long)all.size() - 1ll, (long long)weightSide);
			const auto b_ = rnd.wnext(0ll, (long long)all.size() - 1ll, (long long)weightSide);
			const auto a = all[a_].second;
			const auto b = all[b_].second;
			if (a == b) { continue; }
			if (rnd.next(2)) {
				queries.push_back({a, b});
			} else {
				queries.push_back({b, a});
			}
		}

		finalize(out, n, sets, queries);
	}
};


const int64_t MAX_N = 100000;
const int64_t MINI_BOUNDS = 100;
const int64_t MAX_SUM = 100000;
const int64_t MAX_M = 100000;
const int64_t MAX_Q = 100000;
const int64_t SMALL_BOUNDS = 1000;
const int64_t SMALL_M = 300;
const int64_t SMALL_MAX_SET = 300;

signed main(int argc, char **argv) {
	registerGen(argc, argv, 1);

	// auto full = Subtask("all", 100);

	// for (int64_t i = 6; i <= MAX_M; i *= 1.4) {
	// 	full.withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, BigSetsTestGen::Mode::FIXED, std::nullopt, i);
	// }

	// for (int64_t i = 6; i <= MAX_M; i *= 1.4) {
	// 	full.withTestGen<SimpleTestGen>(1, MAX_N, i, MAX_SUM, MAX_Q, false);
	// 	full.withTestGen<SimpleTestGen>(1, MAX_N, i, MAX_SUM, MAX_Q, true);
	// }

	// generateSubtasks({full});
	// generateGradeProperties({full}, "512", "5.0");
	// return 0;

	const auto subtaskSample = 
		Subtask("Sample", 0).
		withTestGen<FileTestGen>("extras/sample.000.in");

	const auto subtask1 = 
		Subtask("Small all", 14).
		dependsOn(subtaskSample).
		withTestGen<BigSetsTestGen>(3, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::SMALL).
		withTestGen<BigSetsTestGen>(3, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::BIG).
		withTestGen<BigSetsTestGen>(3, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::SQRT).
		withTestGen<BigSetsTestGen>(3, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::EQUAL).
		withTestGen<BigSetsTestGen>(1, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 15).
		withTestGen<BigSetsTestGen>(1, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 22).
		withTestGen<BigSetsTestGen>(1, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 33).
		withTestGen<BigSetsTestGen>(1, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 50).
		withTestGen<BigSetsTestGen>(1, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 75).
		withTestGen<BigSetsTestGen>(1, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 85).
		withTestGen<BigSetsTestGen>(1, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 95).
		withTestGen<BigSetsTestGen>(1, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 105).
		withTestGen<BigSetsTestGen>(1, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 113).
		withTestGen<BigSetsTestGen>(1, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 170).
		withTestGen<BigSetsTestGen>(1, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 256).
		withTestGen<BigSetsTestGen>(1, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 384).
		withTestGen<BigSetsTestGen>(1, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 576).
		withTestGen<BigSetsTestGen>(1, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 864).
		withTestGen<BigSetsTestGen>(1, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 1297).
		withTestGen<SimpleTestGen>(3, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, false).
		withTestGen<SimpleTestGen>(3, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, SMALL_BOUNDS, std::nullopt, std::nullopt, true);

	const auto subtask2 = 
		Subtask("Small n", 12).
		dependsOn(subtaskSample).
		withTestGen<BigSetsTestGen>(3, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::SMALL).
		withTestGen<BigSetsTestGen>(3, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::BIG).
		withTestGen<BigSetsTestGen>(3, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::SQRT).
		withTestGen<BigSetsTestGen>(3, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::EQUAL).
		withTestGen<BigSetsTestGen>(1, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 15).
		withTestGen<BigSetsTestGen>(1, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 22).
		withTestGen<BigSetsTestGen>(1, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 33).
		withTestGen<BigSetsTestGen>(1, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 50).
		withTestGen<BigSetsTestGen>(1, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 75).
		withTestGen<BigSetsTestGen>(1, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 85).
		withTestGen<BigSetsTestGen>(1, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 95).
		withTestGen<BigSetsTestGen>(1, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 105).
		withTestGen<BigSetsTestGen>(1, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 113).
		withTestGen<BigSetsTestGen>(1, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 170).
		withTestGen<BigSetsTestGen>(1, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 256).
		withTestGen<BigSetsTestGen>(1, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 384).
		withTestGen<BigSetsTestGen>(1, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 576).
		withTestGen<BigSetsTestGen>(1, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 864).
		withTestGen<BigSetsTestGen>(1, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 1297).
		withTestGen<SimpleTestGen>(3, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, false).
		withTestGen<SimpleTestGen>(3, MINI_BOUNDS, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, true);

	const auto subtask3 = 
		Subtask("Small k_i", 7).
		dependsOn(subtaskSample).
		withTestGen<BigSetsTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::SMALL).
		withTestGen<BigSetsTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::BIG).
		withTestGen<BigSetsTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::SQRT).
		withTestGen<BigSetsTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::EQUAL).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::FIXED, 15).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::FIXED, 22).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::FIXED, 33).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::FIXED, 50).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::FIXED, 75).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::FIXED, 85).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::FIXED, 95).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::FIXED, 105).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::FIXED, 113).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::FIXED, 170).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::FIXED, 256).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::FIXED, 384).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::FIXED, 576).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::FIXED, 864).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, BigSetsTestGen::Mode::FIXED, 1297).
		withTestGen<SimpleTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, false).
		withTestGen<SimpleTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, MINI_BOUNDS, std::nullopt, true);

	const auto subtask4 = 
		Subtask("Small m", 7).
		dependsOn(subtaskSample).
		withTestGen<BigSetsTestGen>(3, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::SMALL).
		withTestGen<BigSetsTestGen>(3, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::BIG).
		withTestGen<BigSetsTestGen>(3, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::SQRT).
		withTestGen<BigSetsTestGen>(3, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::EQUAL).
		withTestGen<BigSetsTestGen>(1, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 15).
		withTestGen<BigSetsTestGen>(1, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 22).
		withTestGen<BigSetsTestGen>(1, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 33).
		withTestGen<BigSetsTestGen>(1, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 50).
		withTestGen<BigSetsTestGen>(1, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 75).
		withTestGen<BigSetsTestGen>(1, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 85).
		withTestGen<BigSetsTestGen>(1, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 95).
		withTestGen<BigSetsTestGen>(1, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 105).
		withTestGen<BigSetsTestGen>(1, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 113).
		withTestGen<BigSetsTestGen>(1, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 170).
		withTestGen<BigSetsTestGen>(1, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 256).
		withTestGen<BigSetsTestGen>(1, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 384).
		withTestGen<BigSetsTestGen>(1, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 576).
		withTestGen<BigSetsTestGen>(1, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 864).
		withTestGen<BigSetsTestGen>(1, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 1297).
		withTestGen<SimpleTestGen>(3, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, false).
		withTestGen<SimpleTestGen>(3, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, true);

	const auto subtask5 = 
		Subtask("Small set belongs", 12).
		dependsOn(subtaskSample).
		withTestGen<BigSetsTestGen>(2, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, MINI_BOUNDS, BigSetsTestGen::Mode::SMALL).
		withTestGen<BigSetsTestGen>(2, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, MINI_BOUNDS, BigSetsTestGen::Mode::BIG).
		withTestGen<BigSetsTestGen>(2, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, MINI_BOUNDS, BigSetsTestGen::Mode::SQRT).
		withTestGen<BigSetsTestGen>(2, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, MINI_BOUNDS, BigSetsTestGen::Mode::EQUAL).
		withTestGen<SimpleTestGen>(2, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, MINI_BOUNDS, false).
		withTestGen<SimpleTestGen>(2, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, MINI_BOUNDS, true).
		withTestGen<SimpleTestGen>(2, MAX_N, MINI_BOUNDS, MAX_SUM, MAX_Q, std::nullopt, MINI_BOUNDS, true).
		withTestGen<BoundedNodesTestGen>(10, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, MINI_BOUNDS, BoundedNodesTestGen::ModeNodes::SMALL, 10).
		withTestGen<BoundedNodesTestGen>(10, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, MINI_BOUNDS, BoundedNodesTestGen::ModeNodes::BIG, 10).
		withTestGen<BoundedNodesTestGen>(10, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, MINI_BOUNDS, BoundedNodesTestGen::ModeNodes::SMALL, -10).
		withTestGen<BoundedNodesTestGen>(10, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, MINI_BOUNDS, BoundedNodesTestGen::ModeNodes::BIG, -10);

	const auto subtask6 = 
		Subtask("Full", 48).
		dependsOn(subtask3).
		dependsOn(subtask4).
		dependsOn(subtask5).
		dependsOn(subtaskSample).
		withTestGen<BigSetsTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::SMALL).
		withTestGen<BigSetsTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::BIG).
		withTestGen<BigSetsTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::SQRT).
		withTestGen<BigSetsTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::EQUAL).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 15).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 22).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 33).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 50).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 75).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 85).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 95).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 105).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 113).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 170).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 256).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 384).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 576).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 864).
		withTestGen<BigSetsTestGen>(1, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, BigSetsTestGen::Mode::FIXED, 1297).
		withTestGen<SimpleTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, false).
		withTestGen<SimpleTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, std::nullopt, std::nullopt, true).
		withTestGen<Kill2TestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q);

	const std::vector<Subtask> subtasks = {
		subtaskSample,
		subtask1,
		subtask2,
		subtask3,
		subtask4,
		subtask5,
		subtask6
	};

	generateSubtasks(subtasks);
	generateGradeProperties(subtasks, "512", "5.0");

	// const auto subtaskSmallSets = 
	// 	Subtask("Small sets", 20).
	// 	dependsOn(subtaskSample).
	// 	withTestGen<BigSetsTestGen>(10, MAX_N, MAX_M, MAX_SUM, MAX_Q, BigSetsTestGen::Mode::SMALL, SMALL_MAX_SET).
	// 	withTestGen<BigSetsTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, BigSetsTestGen::Mode::BIG, SMALL_MAX_SET).
	// 	withTestGen<BigSetsTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, BigSetsTestGen::Mode::SQRT, SMALL_MAX_SET).
	// 	withTestGen<BigSetsTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, BigSetsTestGen::Mode::EQUAL, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(2, MAX_N, MAX_M, MAX_SUM, MAX_Q, true, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(2, MAX_N, MAX_M, MAX_SUM, MAX_Q, false, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(1, MAX_N, 500, MAX_SUM, MAX_Q, false, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(1, MAX_N, 500, MAX_SUM, MAX_Q, true, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(1, MAX_N, 400, MAX_SUM, MAX_Q, false, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(1, MAX_N, 400, MAX_SUM, MAX_Q, true, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(1, MAX_N, 330, MAX_SUM, MAX_Q, false, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(1, MAX_N, 330, MAX_SUM, MAX_Q, true, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(1, MAX_N, 200, MAX_SUM, MAX_Q, false, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(1, MAX_N, 200, MAX_SUM, MAX_Q, true, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(1, MAX_N, 80, MAX_SUM, MAX_Q, false, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(1, MAX_N, 80, MAX_SUM, MAX_Q, true, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(1, MAX_N, 25, MAX_SUM, MAX_Q, false, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(1, MAX_N, 25, MAX_SUM, MAX_Q, true, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(1, MAX_N, 10, MAX_SUM, MAX_Q, false, SMALL_MAX_SET).
	// 	withTestGen<SimpleTestGen>(1, MAX_N, 10, MAX_SUM, MAX_Q, true, SMALL_MAX_SET);

	// const auto subtaskSmallQ = 
	// 	Subtask("Small m", 20).
	// 	dependsOn(subtaskSample).
	// 	withTestGen<BigSetsTestGen>(10, MAX_N, SMALL_M, MAX_SUM, MAX_Q, BigSetsTestGen::Mode::SMALL).
	// 	withTestGen<BigSetsTestGen>(3, MAX_N, SMALL_M, MAX_SUM, MAX_Q, BigSetsTestGen::Mode::BIG).
	// 	withTestGen<BigSetsTestGen>(3, MAX_N, SMALL_M, MAX_SUM, MAX_Q, BigSetsTestGen::Mode::SQRT).
	// 	withTestGen<BigSetsTestGen>(3, MAX_N, SMALL_M, MAX_SUM, MAX_Q, BigSetsTestGen::Mode::EQUAL).
	// 	withTestGen<SimpleTestGen>(3, MAX_N, SMALL_M, MAX_SUM, MAX_Q, false).
	// 	withTestGen<SimpleTestGen>(3, MAX_N, SMALL_M, MAX_SUM, MAX_Q, true).
	// 	withTestGen<SimpleTestGen>(3, MAX_N, 200, MAX_SUM, MAX_Q, false).
	// 	withTestGen<SimpleTestGen>(3, MAX_N, 200, MAX_SUM, MAX_Q, true);

	// const auto subtaskFull =
	// 	Subtask("Full", 45).
	// 	dependsOn(subtaskSmall).
	// 	dependsOn(subtaskSmallSets).
	// 	dependsOn(subtaskSmallQ).
	// 	withTestGen<BigSetsTestGen>(10, MAX_N, MAX_M, MAX_SUM, MAX_Q, BigSetsTestGen::Mode::SMALL).
	// 	withTestGen<BigSetsTestGen>(5, MAX_N, MAX_M, MAX_SUM, MAX_Q, BigSetsTestGen::Mode::BIG).
	// 	withTestGen<BigSetsTestGen>(5, MAX_N, MAX_M, MAX_SUM, MAX_Q, BigSetsTestGen::Mode::SQRT).
	// 	withTestGen<BigSetsTestGen>(5, MAX_N, MAX_M, MAX_SUM, MAX_Q, BigSetsTestGen::Mode::EQUAL).
	// 	withTestGen<Kill2TestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q).
	// 	withTestGen<SimpleTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, false).
	// 	withTestGen<SimpleTestGen>(3, MAX_N, MAX_M, MAX_SUM, MAX_Q, true).
	// 	withTestGen<SimpleTestGen>(3, MAX_N, 330, MAX_SUM, MAX_Q, false).
	// 	withTestGen<SimpleTestGen>(3, MAX_N, 330, MAX_SUM, MAX_Q, true).
	// 	withTestGen<SimpleTestGen>(3, MAX_N, 450, MAX_SUM, MAX_Q, false).
	// 	withTestGen<SimpleTestGen>(3, MAX_N, 450, MAX_SUM, MAX_Q, true).
	// 	withTestGen<SimpleTestGen>(3, MAX_N, 1000, MAX_SUM, MAX_Q, false).
	// 	withTestGen<SimpleTestGen>(3, MAX_N, 1000, MAX_SUM, MAX_Q, true);
}
