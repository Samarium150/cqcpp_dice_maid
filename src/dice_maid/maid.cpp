#include "maid.h"

using namespace std;


string Maid::setting = "";


void Maid::init() {
	string data = get_data_path();

	if (data != "") {
		stringstream s;
		s << data << "\\setting";

		Maid::setting = s.str();

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
	info << "Change Log: " << endl;
	info << "1. deprecatre jrrp function;" << endl;
	info << "use #help for manual page";
	return info.str();
}

string Maid::get_help() {
	stringstream help;
	help << "Manual Page: " << endl;
	help << "1. #maid: What's new;" << endl;
	help << "2. #help: Manual Page;" << endl;
	help << "3. #jrrp: Today's fortune; *temporarily deprecated" << endl;
	help << "4. #r dice_expression: Roll a given dice, default number is 1; default roll#rd=r1d100;" << endl;
	help << "5. #ra [skill] skill_point: check a given skill, example: #ra dodge 50;" << endl;
	help << "6. #sc San_expression Sanity: quick San check, example: #sc 1d10/1d100 80;" << endl;
	help << "7. #h dice_expression: roll a hidden dice, default is 1d100;" << endl;
	help << "8. #tarot [reason]: get a tarot card;" << endl;
	help << "9. #ti/li: decide madness;" << endl;
	help << "10. #coc7 [integer n]：generate n groups of ;" << endl;
	help << "11. #en [skill] skill_point: enforce a skill: #en persuade 50;" << endl;
	help << "12. #rb[integer n] [skill] skill_point: skill checking with bonus n dice;" << endl;
	help << "13. #rp[integer n] [skill] skill_point: skill checking with punishment n dice;" << endl;
	help << "14. #set rate 0/1: set checking rate" << endl;
	help << "15. #!dissmiss QQ: leave the group";
	return help.str();
}

string Maid::get_zbzm(const int64_t id){
	stringsteam zbzm;
	zbzm << "揍爆了字母，希望ta升高的血压能因此降低一些呢" << endl;
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
			} catch (const invalid_argument& ia) { return "invalid expression"; }
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
		string response = (set == 0) ? string("set rate as standard") : string("set rate as double");
		return response;
	}

	regex repeat("^#fd\\s(0|1)$");
	smatch m_rep;
	if (regex_match(msg, m_rep, repeat)) {
		int repeat = stoi(extract(msg, 1, ' '));
		string response = (repeat == 0) ? string("repeat off") : string("repeat on");
		return response;
	}
//zbzm
	regex zbzm("^zbzm")
	smatch m_zbzm;
	if(regex_match(msg, m_zbzm,zbzm)) return Maid::get_zbzm(id);

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