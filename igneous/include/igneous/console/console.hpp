#pragma once

#include "igneous/console/conVar.hpp"

#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

#include <imgui.h>

namespace igneous {
#define RETURN_EXISTS(name) if (exists(name)) { IG_CONSOLE_ERROR("A token already exists with the name: {}", name); return; }
#define RETURN_INVALID(name) if (!isValid(name)) { IG_CONSOLE_ERROR("Token name is invalid."); return; }

typedef std::vector<std::string> arg_list;
typedef void(*command_callback)(const std::string&, const arg_list&);

enum CF_enum
{
	CF_NONE = 0,
	CF_TOGGLE = 1 << 0,
	CF_ONOFF = 1 << 1
};

struct call
{
	std::string name;
	arg_list args;
	int flags = CF_NONE;
};

typedef std::vector<call> call_sequence;

class Console
{
public:
	static Console& getInstance();

	const ImColor RED = ImColor(255, 0, 0);
	const ImColor GREEN = ImColor(0, 255, 0);
	const ImColor BLUE = ImColor(0, 0, 255);
	const ImColor CYAN = ImColor(0, 255, 255);
	const ImColor WHITE = ImColor(255, 255, 255);
	const ImColor YELLOW = ImColor(255, 255, 0);

	enum level_enum
	{
		trace,
		debug,
		info,
		warn,
		err,
		critical,
		off,
		NUM_LEVELS
	};

	void alias(const std::string& name, const std::string& exe);
	bool aliasExists(const std::string& name) const;
	void bind(int key, const std::string& exe);
	bool bindExists(int key) const;
	void clear();
	void command(const std::string& name, command_callback callback);
	bool commandExists(const std::string& name) const;
	void execute(call_sequence calls, bool positive = true);
	void execute(const std::string& input, bool record = false, bool positive = true);
	bool exists(const std::string& name) const;
	static call_sequence parse(std::string input);
	void printInfo() const;
	ConVar& get(const std::string& variable);
	bool isValid(const std::string& name) const;
	static void onKey(int key, int scancode, int action, int mods);
	void remove(const std::string& name);
	void render();
	void run(const std::string& command, arg_list args = {});
	void runBind(int key, bool positive = true);
	void runFile(const std::string& filePath);
	void unbind(int key);
	static std::string unparse(call_sequence calls);
	ConVar& variable(const std::string& name, float defaultValue, convar_callback callback = nullptr);
	ConVar& variable(const std::string& name, float defaultValue, float initialValue, convar_callback callback = nullptr);
	bool variableExists(const std::string& variable) const;
	void write(const std::string& contents, level_enum level);

	Console(Console const&) = delete;
	void operator=(Console const&) = delete;
private:
	Console();
	~Console() {}

	int textEditCallback(ImGuiInputTextCallbackData* data);
	static int textEditCallbackStub(ImGuiInputTextCallbackData* data);

	static void aliasCallback(const std::string& name, const arg_list& args);
	static void bindCallback(const std::string& name, const arg_list& args);
	static void clearCallback(const std::string& name, const arg_list& args);
	static void helpCallback(const std::string& name, const arg_list& args);
	static void printCallback(const std::string& name, const arg_list& args);
	static void runFileCallback(const std::string& name, const arg_list& args);
	static void unbindCallback(const std::string& name, const arg_list& args);

	struct line
	{
		ImColor color;
		std::string contents;
	};

	const unsigned int max_lines = 100;
	bool scrollToBottom = false;

	static const unsigned int max_history = 100;
	int history_index = -1;
	static const unsigned int max_input = 256;
	std::string input;
	std::string editing;

	std::unordered_map<std::string, call_sequence> aliases;
	std::unordered_map<int, call_sequence> binds;
	std::unordered_map<std::string, command_callback> commands;
	ImColor colors[level_enum::NUM_LEVELS];
	std::deque<std::string> history;
	std::deque<line> lines;
	std::unordered_map<std::string, ConVar> variables;

	ConVar& consoleVar;
};
} // end namespace igneous