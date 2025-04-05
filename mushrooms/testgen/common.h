#pragma once

#include "testlib.h"
#include <filesystem>

#define myAssert(test, message) \
	if (!(test)) { \
		std::cerr << "Assertion ( " << (#test) << ") <=> " << message << " failed at line " << __LINE__ << std::endl; \
		exit(0); \
	}

std::pair<
	std::string,
	std::string
> formatFileNames(const int test, const int digits) {
	// TODO: fix
	std::string testIndex = vtos(test);
	while (testIndex.size() < digits) { testIndex = "0" + testIndex; }

	myAssert(testIndex.size() <= digits, " index needs to fit into digits");

	const std::string testFileName = std::string("tests/sets.") + testIndex;

	std::cerr << "---- Generating test case " << testFileName << "\n";

	return {testFileName + ".in", testFileName + ".out"};
}

std::string gradeProperties() {
	const std::string fileName = "grade.properties";
	std::cerr << "Generating grade.properties " << "\n";
	return fileName;
}

struct BaseInputStructure {
	virtual void validate(std::istream& in) const { }
};

struct BaseTestGen  {
	int64_t count;

	BaseTestGen(const int64_t count) : count(count) { }

	virtual std::string name() const { return typeid(*this).name(); }
	virtual BaseInputStructure inputStructure() { return BaseInputStructure(); }
	virtual void generate(std::ostream& out) = 0;
	virtual ~BaseTestGen() = default;
};

struct FileTestGen : BaseTestGen {
	std::filesystem::path path;

	FileTestGen(
		const std::filesystem::path& path
	) : BaseTestGen(1), path(path) { }

	void generate(std::ostream& out) {
		// TODO - optimise
		std::ifstream fIn(path);
		const std::string fileContents { 
			std::istreambuf_iterator<char>(fIn),
			std::istreambuf_iterator<char>()
		};
		out << fileContents; std::flush(out);
	}
};

struct Subtask {
	static int64_t globalSubtaskIndex;

	mutable int64_t start = 0;
	mutable int64_t end = 0;

	int64_t subtaskIndex;

	std::string name;
	int64_t points;

	std::vector<std::shared_ptr<BaseTestGen>> testGens;
	std::vector<int64_t> dependencies;

	Subtask(
		const std::string name, 
		const int64_t points
	) : subtaskIndex(globalSubtaskIndex ++), name(name), points(points), testGens() {}

	template<class T, class... Ts>
	Subtask& withTestGen(Ts... args) {
		testGens.emplace_back(
			static_cast<BaseTestGen*>(new T(std::forward<Ts>(args)...))
		);
		return *this;
	}

	Subtask& dependsOn(const Subtask& other) {
		dependencies.push_back(other.subtaskIndex);
		return *this;
	}
};
int64_t Subtask::globalSubtaskIndex = 0;

void generateSubtasks(const std::vector<Subtask> &subtasks) {
	// TODO: validation
	int64_t testInd = 1;
	int64_t totalPoints = 0;
	
	for (int64_t i = 0; i < subtasks.size(); i ++) {
		myAssert(i == subtasks[i].subtaskIndex, " subtask indexing invariants violated");
		totalPoints += subtasks[i].points;
	}
	myAssert(totalPoints == 100, " point over all subtasks should sum to 100");

	for (const auto &st : subtasks) {
		st.start = testInd;
		std::cerr << "Started generating subtask " << st.name << "\n";
		for (const auto &tg : st.testGens) {
			std::cerr << "-- Generating with test generator " << tg->name() << "\n";
			for (int64_t i = 0; i < tg->count; i ++) {
				auto files = formatFileNames(testInd ++, 3);
				{
					std::ofstream input(files.first);
					tg->generate(input);
				}
				{
					const auto inputStructure = tg->inputStructure();
					std::ifstream input(files.first);
					inputStructure.validate(input);
				}
				{
					const std::string command = "./bin/author_checks < " + files.first + " > " + files.second;
					std::system(command.c_str());
				}
			}
		}
		myAssert(testInd != st.start, " empty subtask");
		st.end = testInd - 1;
		totalPoints += st.points;
	}
}

void generateGradeProperties(
	const std::vector<Subtask>& subtasks,
	const std::string memory,
	const std::string time
) {
	// TODO: checks
	const auto gprop = gradeProperties();
	std::ofstream out(gprop);

	out << "memory=" << memory << "\n";
	out << "time=" << time << "\n";

	out << "weights=";
	for (int64_t i = 0; i < subtasks.size(); i ++) {
		out << subtasks[i].points;
		if (i + 1 != subtasks.size()) {
			out << ",";
		} else {
			out << "\n";
		}
	}

	out << "groups=";
	for (int64_t i = 0; i < subtasks.size(); i ++) {
		out << subtasks[i].start << "-" << subtasks[i].end;
		if (i + 1 != subtasks.size()) {
			out << ",";
		} else {
			out << "\n";
		}
	}

	out << "dependencies=";
	for (int64_t i = 0; i < subtasks.size(); i ++) {
		const auto &st = subtasks[i];
		for (int64_t j = 0; j < st.dependencies.size(); j ++) {
			out << st.dependencies[j];
			if (j + 1 != st.dependencies.size()) {
				out << ";";
			}
		}
		if (i + 1 != subtasks.size()) {
			out << ",";
		} else {
			out << "\n";
		}
	}
}
