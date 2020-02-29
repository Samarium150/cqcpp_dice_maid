#include "maid.h"

using namespace std;

string Maid::setting = "";

void Maid::init() {
	string data = get_data_path();

	if (data != "") {
		stringstream s;
		s << data << "\\setting";
		Maid::setting = s.str();
#ifdef STD
		if (_access(Maid::setting.c_str(), 0) == -1 && (!CreateDirectory(Maid::setting.c_str(), NULL))) throw - 1;
#endif
		Dice::init();
		Fortune::init();
		Madness::init();
		Tarot::init();
	} else throw -1;
}

string Maid::get_info() {
	stringstream info;
	info << get_file("resources/maid.txt").begin();
	return info.str();
}

string Maid::get_help() {
	stringstream help;
	help << get_file("resources/manual.txt").begin();
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
	if (regex_match(msg, m_tr, tr)) {
		if (regex_search(msg, m_rs, rs)) {
			auto x = m_rs.begin();
			string str = x->str();
			return Tarot::get_card(strip(str));
		} else
			return Tarot::get_card();
	}

	regex ma("^#ti$"), mb("^#li$");
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
			} catch (const invalid_argument&) { return "表达式错误"; }
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