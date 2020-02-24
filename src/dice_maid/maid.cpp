#include "maid.h"

using namespace std;

string Maid::banned = "";
string Maid::cursed = "";
string Maid::setting = "";
int64_t Maid::master = 941295333L;

void Maid::init() {
	string data = get_data_path();

	if (data != "") {
		stringstream b, c, s;
		b << data << "\\banned";
		c << data << "\\cursed";
		s << data << "\\setting";
		Maid::banned = b.str();
		Maid::cursed = c.str();
		Maid::setting = s.str();
		if (_access(Maid::banned.c_str(), 0) == -1) CreateDirectory(Maid::banned.c_str(), NULL);
		if (_access(Maid::cursed.c_str(), 0) == -1) CreateDirectory(Maid::cursed.c_str(), NULL);
		if (_access(Maid::setting.c_str(), 0) == -1) CreateDirectory(Maid::setting.c_str(), NULL);
	}
	Dice::init();
	Fortune::init();
	Tarot::init();
}

string Maid::get_info() {
	stringstream info;
	info << "Dice Maid by Samarium/941295333" << endl;
	info << "Version: v1.2.7" << endl;
	info << "更新日志: " << endl;
	info << "1. 因为文件储存的原因暂时取消了jrrp功能;" << endl;
	info << "使用#help查看帮助文档";
	return info.str();
}

string Maid::get_help() {
	stringstream help;
	help << "帮助文档: " << endl;
	help << "1. #maid: 查看更新信息;" << endl;
	help << "2. #help: 查看帮助文档;" << endl;
	help << "3. #jrrp: 查看今日人品; *temporarily deprecated" << endl;
	help << "4. #r骰子表达式: 投一个指定的骰子, 不写骰子个数则默认为1;默认#rd=r1d100;" << endl;
	help << "5. #ra [检定项目] [数值]: 进行一次指定技能的检定, 命令、项目和技能数值之间必须要用空格隔开, 示例: #ra 侦察 50;" << endl;
	help << "6. #sc [San值表达式] [当前San值]: 快捷San check, 示例: #sc 1d10/1d100 80;" << endl;
	help << "7. #h骰子表达式: 进行一次暗骰, 默认为1d100;" << endl;
	help << "8. #tarot [理由]: 抽一张塔罗牌;" << endl;
	help << "9. #ti/li: 决定疯狂症状;" << endl;
	help << "10. #coc7 [正整数n]：生成n组7版人物数据，最多10组;" << endl;
	help << "11. #en [检定项目] [数值]: 进行一次指定技能的成长检定, 命令、项目和技能数值之间必须要用空格隔开, 示例: #en 侦察 50;" << endl;
	help << "12. #rb[正整数n] [检定项目] [数值]: 进行一次附加一个/n个奖励骰的指定技能的检定;" << endl;
	help << "13. #rp[正整数n] [检定项目] [数值]: 进行一次附加一个/n个惩罚骰的指定技能的检定;" << endl;
	help << "14. #set rate 0/1: 设置技能检定倍率（有无倍增），默认为规则书标准倍率" << endl;
	help << "15. #!dissmiss 骰娘QQ号: 让骰娘退群";
	return help.str();
}

string Maid::command(int64_t id, string msg) {
	regex info("^(#|>|~|.|-)maid$");
	smatch m_info;
	if (regex_match(msg, m_info, info)) return Maid::get_info();

	regex help("^(#|>|~|.|-)help$");
	smatch m_help;
	if (regex_match(msg, m_help, help)) return Maid::get_help();

	regex coc("^#coc7\\s*\\d*"), ncoc("\\s+\\d+");
	smatch m_coc, m_ncoc;
	if (regex_match(msg, m_coc, coc)) {
		if (regex_search(msg, m_ncoc, ncoc)) {
			auto x = m_ncoc.begin();
			string str = x->str();
			int c = stoi(strip(str));
			return Dice::coc(c);
		} else
			return Dice::coc();
	}

	regex ft("^#jrrp$");
	smatch m_ft;
	if (regex_match(msg, m_ft, ft)) return Fortune::get_luck(id);

	regex tr("^#tarot.*"), rs("\\s.*");
	smatch m_tr, m_rs;
	if (regex_match(msg, m_tr, tr) && !check_cursed(id)) {
		if (regex_search(msg, m_rs, rs)) {
			auto x = m_rs.begin();
			string str = x->str();
			return Tarot::get_card(strip(str));
		} else
			return Tarot::get_card();
	}

	regex ma("^#li$"), mb("^#ti$");
	smatch m_ma, m_mb;
	if (regex_match(msg, m_ma, ma))
		return Madness::get_symptom(0);
	else if (regex_match(msg, m_mb, mb))
		return Madness::get_symptom(1);

	regex ra("^#ra.*"), en("^#en.*"), rb("^#rb\\d*.*"), rp("^#rp\\d*.*");
	regex ra_num("\\s\\d+(/\\d+)*"), ra_reason("\\s\\D+"), rbpc("(b|p)\\d+\\s*");
	smatch m_ra, m_en, m_rb, m_rp, m_ra_num, m_ra_reason, m_rbpc;
	string reason;
	int num, count;
	if (regex_search(msg, m_ra_reason, ra_reason)) {
		auto x = m_ra_reason.begin();
		reason = x->str();
		reason = strip(reason);
	} else
		reason = string("");
	if (regex_search(msg, m_ra_num, ra_num)) {
		auto y = m_ra_num.begin();
		string temp = y->str();
		vector<string> s = split(strip(temp), '/');
		if (s.size() == 2) {
			try {
				num = (int)floor(stoi(s[0]) / stoi(s[1]));
			} catch (const invalid_argument& ia) { return "表达式错误"; }
		} else
			num = stoi(s[0]);
	} else
		num = 20;
	if (regex_search(msg, m_rbpc, rbpc)) {
		auto z = m_rbpc.begin();
		string tp = z->str();
		tp = strip(tp);
		string n = (start_with(tp, "b")) ? extract(tp, 1, 'b') : extract(tp, 1, 'p');
		count = stoi(n);
	} else
		count = 1;
	int rate = get_rate(id);
	if (regex_match(msg, m_ra, ra))
		return Dice::check(reason, num, rate);
	else if (regex_match(msg, m_en, en))
		return Dice::check(reason, num, rate, 2);
	else if (regex_match(msg, m_rb, rb))
		return Dice::check(reason, num, rate, 3, count);
	else if (regex_match(msg, m_rp, rp))
		return Dice::check(reason, num, rate, 4, count);

	regex rd("^#r.*"), hd("^#h.*");
	smatch m_rd, m_hd;
	if (regex_match(msg, m_rd, rd) || regex_match(msg, m_hd, hd)) return Dice::toss(msg);

	regex sc("^#sc\\s(\\d+d\\d+\\+\\d+|\\d*d\\d+|\\d+)/(\\d+d\\d+\\+\\d+|\\d*d\\d+|\\d+)\\s\\d+$");
	smatch m_sc;
	if (regex_match(msg, m_sc, sc)) return Dice::san_check(msg, rate);

	regex set("^#set\\srate\\s(0|1)$");
	smatch m_set;
	if (regex_match(msg, m_set, set)) {
		int set = stoi(extract(msg, 2, ' '));
		set_rate(id, set);
		string response = (set == 0) ? string("将倍率设置为标准倍率") : string("将倍率设置为翻倍倍率");
		return response;
	}

	return string("");
}

string Maid::command(int64_t id, string name, string msg) {
	stringstream pre;
	regex prefix("^(#|>|~|.|-)(maid|help)");
	smatch m_pre;
	if (!regex_match(msg, m_pre, prefix)) pre << name;
	string result = Maid::command(id, msg);
	if (result != "")
		pre << result;
	else
		pre.str("");
	return pre.str();
}

string Maid::ban(int64_t id) {
	stringstream ss, response;
	ss << Maid::banned << "\\" << id << ".txt";
	const char* path = ss.str().c_str();
	if (check_banned(id)) {
		HANDLE file = CreateFile(
			path,
			GENERIC_READ,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
		(file == INVALID_HANDLE_VALUE) ? response << "生成文件失败, " << GetLastError() << ", " << ss.str() 
			: response << "banned " << id;
		CloseHandle(file);
	} else {
		response << "already banned " << id << ", " << ss.str();
	}
	return response.str();
}

string Maid::unban(int64_t id) {
	stringstream ss, response;
	ss << Maid::banned << "\\" << id << ".txt";
	if (check_banned(id)) {
		if (remove(ss.str().c_str()) == 0)
			response << "unbanned " << id;
		else
			response << "移除文件失败";
	} else {
		response << id << " is not banned";
	}
	return response.str();
}

bool Maid::check_banned(const int64_t id) {
	stringstream ss;
	ss << Maid::banned << "\\" << id << ".txt";
	return _access(ss.str().c_str(), 0) == 0;
}

bool Maid::check_cursed(const int64_t id) {
	stringstream ss;
	ss << Maid::cursed << "\\" << id << ".txt";
	return _access(ss.str().c_str(), 0) == 0;
}

int Maid::get_rate(int64_t id) {
	stringstream ss;
	ss << Maid::setting << "\\" << id << ".txt";
	const char* path = ss.str().c_str();
	if (_access(path, 0) == 0) {
		ifstream file;
		file.open(path);
		char buffer[256];
		file.getline(buffer, 256);
		file.close();
		return (atoi(buffer) >= 1) ? 1 : 0;
	} else
		return 0;
}

void Maid::set_rate(int64_t id, int set) {
	stringstream ss;
	ss << Maid::setting << "\\" << id << ".txt";
	const char* path = ss.str().c_str();
	if (_access(path, 0) == 0) remove(path);
	ofstream file;
	file.open(path);
	(set == 0) ? file << 0 : file << 1;
	file.close();
}