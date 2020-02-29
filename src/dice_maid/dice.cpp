#include "dice.h"

using namespace std;

void Dice::init() {
	time_t t;
	time(&t);
	struct tm temp;
	localtime_s(&temp, &t);
	
	int skipper = (int)(temp.tm_min * (temp.tm_mon + 1)
		% ((temp.tm_hour + 1) * (temp.tm_year + 1900)) 
		* (temp.tm_sec - temp.tm_mday));
		
	for (int i = 0; i < skipper; i++) common::random(100);
}

int Dice::roll(int times, int sides, int div = 1) {
	int result = 0;
	for (int i = 1; i <= times; i++) result += common::random(sides);
	return (int)floor(result / div);
}

string Dice::roll(int times, int sides, int div, bool output, int& s) {
	int	r = 0;
	stringstream result;
	if (output) result << "(";
	for (int i = 1; i <= times; i++) {
		int get = common::random(sides);
		r += get;
		result << get;
		if (i != times) result << "+";
	}
	if (output) result << ")";
	if (div != 1) result << "/" << div;
	if (output)
		result << "=" << floor(r / div);
	else
		s += r;
	return result.str();
}

string Dice::coc() {
	stringstream result;
	int	strength = Dice::roll(3, 6) * 5;
	int	constitution = Dice::roll(3, 6) * 5;
	int size = (Dice::roll(2, 6) + 6) * 5;
	int	dexterity = Dice::roll(3, 6) * 5;
	int	appearance = Dice::roll(3, 6) * 5;
	int	intelligence = (Dice::roll(2, 6) + 6) * 5;
	int	power = Dice::roll(3, 6) * 5;
	int	education = (Dice::roll(2, 6) + 6) * 5;
	int	luck = Dice::roll(3, 6) * 5;
	int	total = strength + constitution + size 
		+ dexterity + appearance + intelligence 
		+ power + education + luck;
	result << "调查员作成Ex:" << endl;
	result << "力量STR: " << strength << "，体质CON: " << constitution << "，体型SIZ: " << size << "，" << endl;
	result << "敏捷DEX: " << dexterity << "，外貌APP: " << appearance << "，智力INT: " << intelligence << "，" << endl;
	result << "意志POW: " << power << "，教育EDU: " << education << "，幸运LUC: " << luck << "，" << endl;
	result << "属性总和: " << total;
	return result.str();
}

string Dice::coc(int n) {
	if (n > 10)
		return "最多生成10组数据";
	else if (n <= 0)
		return "";

	stringstream result;
	for (int i = 0; i < n; i++) {
		result << Dice::coc();
		if (i != n - 1) result << endl;
	}
	return result.str();
}

string Dice::check_result(int result, int skill, int type) {
	stringstream r;
	int	half  = ( int )floor(0.5 * skill);
	int	fifth = ( int )floor(0.2 * skill);
	if (type == 0) {
		if (result > skill) {
			if ((skill < 50 && result > 95) || result == 100)
				r << "大失败";
			else
				r << "失败";
		} else if (half < result && result <= skill)
			r << "普通成功";
		else if (fifth < result && result <= half)
			r << "困难成功";
		else if (result <= fifth && result > 1)
			r << "极难成功";
		else if (result == 1)
			r << "大成功";
	} else if (type == 1) {
		if (result > 95 && result > skill)
			r << "大失败";
		else if (result <= 5 && skill > result)
			r << "大成功";
		else if (skill < result && result <= 95)
			r << "失败";
		else if (half < result && result <= skill)
			r << "普通成功";
		else if (fifth < result && result <= half)
			r << "困难成功";
		else if (result <= fifth)
			r << "极难成功";
	} else if (type == 2) {
		if (result > skill || result > 95)
			r << "成功";
		else
			r << "失败";
	}
	return r.str();
}

string Dice::toss(string msg, int type, int& s) {
	stringstream result;
	if (type == 0) {
		regex default_default("\\Dd"), default_num("\\Dd\\d+"), num_default("\\d+d");
		regex num_num("\\d+d\\d+"), dived("/\\d+");

		smatch m_dd, m_dn, m_nd, m_nn, m_d;
		int	n = 1, d = 0, r = 0, div = 1;

		string temp;
		if (regex_search(msg, m_d, dived)) {
			auto y = m_d.begin();
			string dv = y->str();
			div	= stoi(extract(dv, 1, '/'));
		}
		if (regex_search(msg, m_nn, num_num)) {
			auto x = m_nn.begin();
			temp = x->str();
			n = stoi(extract(temp, 0, 'd'));
			d = stoi(extract(temp, 1, 'd'));
		} else if (regex_search(msg, m_dn, default_num)) {
			auto x = m_dn.begin();
			temp = x->str();
			d = stoi(extract(temp, 1, 'd'));
		} else if (regex_search(msg, m_nd, num_default)) {
			auto x = m_nd.begin();
			temp = x->str();
			n = stoi(extract(temp, 0, 'd'));
			d = 100;
		} else if (regex_search(msg, m_dd, default_default) || msg == "#h")
			d = 100;

		if (n > 50) throw n;
		result << "投" << n << "d" << d;
		if (div != 1) result << "/" << div;
		result << "=" << Dice::roll(n, d, div, true, s);
	} else {
		regex default_default("^d"), default_num("^d\\d+"), num_default("^\\d+d");
		regex num_num("^\\d+d\\d+"), num("^\\d+");

		smatch m_dd, m_dn, m_nd, m_nn, m_d;
		int	n = 1, d = 0, r = 0, div = 1;
		string temp;

		if (regex_search(msg, m_nn, num_num)) {
			auto x = m_nn.begin();
			temp = x->str();
			n = stoi(extract(temp, 0, 'd'));
			d = stoi(extract(temp, 1, 'd'));
		} else if (regex_search(msg, m_dn, default_num)) {
			auto x = m_dn.begin();
			temp = x->str();
			d = stoi(extract(temp, 1, 'd'));
		} else if (regex_search(msg, m_nd, num_default)) {
			auto x = m_nd.begin();
			temp = x->str();
			n = stoi(extract(temp, 0, 'd'));
			d = 100;
		} else if (regex_search(msg, m_dd, default_default))
			d = 100;
		else if (regex_match(msg, m_d, num)) {
			auto x = m_d.begin();
			temp = x->str();
			n = stoi(temp);
			result << n;
			if (type == 2) s += n;
			return result.str();
		}
		if (n > 50) throw n;
		if (type == 1)
			result << n << "d" << d;
		else
			result << Dice::roll(n, d, div, false, s);
	}

	return result.str();
}

string Dice::toss(string msg) {
	stringstream result;
	int sum = 0, div = 1, &s = sum;
	regex multi("(\\d*d\\d*)(\\+((\\d*d\\d*)|(\\d+)))+"), rs("\\s.*"), dived("/\\d+");
	smatch m_multi, m_rs, m_dv;
	string reason("");
	if (regex_search(msg, m_rs, rs)) {
		auto z = m_rs.begin();
		string temp = z->str();
		reason = string(strip(temp));
	}
	if (reason != "") result << "因为" << reason << ", ";

	if (regex_search(msg, m_multi, multi)) {
		auto x = m_multi.begin();
		string temp = x->str();
		vector<string> v = split(temp, '+');

		if (regex_search(msg, m_dv, dived)) {
			auto y = m_dv.begin();
			string tp = y->str();
			div = stoi(extract(tp, 1, '/'));
		}
		result << "投";
		if (div != 1) result << "(";
		try {
			for (int i = 0; i < v.size(); i++) {
				result << Dice::toss(v[i], 1, s);
				if (i != v.size() - 1) result << "+";
			}
		} catch (int) {
			result.str("");
			result << "不要扔这么多骰子!";
			return result.str();
		} catch (const logic_error&) {
			result.str("");
			result << "表达式错误";
			return result.str();
		}
		if (div != 1)
			result << ")/" << div;

		result << "=" << "(";
		for (int i = 0; i < v.size(); i++) {
			result << Dice::toss(v[i], 2, s);
			if (i != v.size() - 1) result << "+";
		}
		result << ")";
		if (div != 1) 
			result << "/" << div;

		result << "=" << floor(sum / div);
		return result.str();
	}
	try {
		result << Dice::toss(msg, 0, s);
	} catch (int) {
		result.str("");
		result << "不要扔这么多骰子!";
		return result.str();
	}
	return result.str();
}

string Dice::check(string msg, int num, int rate, int type) {
	stringstream result;
	int	d = Dice::roll(1, 100);
	result << "进行" << msg;
	if (type <= 1) {
		result << "检定: 1d100=" << d << "|" << num << ", " << Dice::check_result(d, num, rate);
	} else if (type == 2) {
		string temp = Dice::check_result(d, num, type);
		result << "成长检定: 1d100=" << d << "|" << num << ", " << temp;
		if (temp == "成功") {
			int dx = Dice::roll(1, 10);
			result << endl << "增长: 1d10=" << dx << ", " << endl << "最终为: " << num + dx;
		}
	}
	return result.str();
}

string Dice::check(string msg, int num, int rate, int type, int count) {
	if (count > 5) return "不要扔这么多骰子!";

	stringstream result;
	result << "进行" << msg;
	int d = Dice::roll(1, 100), ori = (d - d % 10) / 10;
	result << "检定: 1d100=" << d << ", " << endl;
	if (type == 3)
		result << "奖励骰: ";
	else
		result << "惩罚骰: ";

	int* p = new int[count];
	for (int i = 1; i <= count; i++) {
		int x = Dice::roll(1, 10);
		p[i - 1] = (x == 10 && ori != 10) ? 0 : x;
		result << x << ", ";
	}
	result << endl << "最终结果: ";
	int r = (type == 3) ? *min_element(p, p + count) : *max_element(p, p + count);
	if ((type == 3 && (r < ori)) || (type == 4 && (r > ori))) d = d % 10 + r * 10;
	if (d == 0) d = 100;
	result << d << "|" << num << ", " << Dice::check_result(d, num, rate);
	delete[] p;
	return result.str();
}

string Dice::san_check(string msg, int rate) {
	stringstream   result;
	regex dc("\\s(\\d+d\\d+\\+\\d+|\\d*d\\d+|\\d+)/(\\d+d\\d+\\+\\d+|\\d*d\\d+|\\d+)");
	regex san("\\s\\d+$");
	smatch m_dc, m_san;
	vector<string> dices;
	if (regex_search(msg, m_dc, dc)) {
		auto x = m_dc.begin();
		string temp = x->str();
		dices = split(strip(temp), '/');
	} else
		return "";
	int sanity = 0;
	if (regex_search(msg, m_san, san)) {
		auto y = m_san.begin();
		string tp = y->str();
		sanity = stoi(strip(tp));
	} else
		return "";
	if (dices.size() != 2)
		result << "表达式错误";
	else {
		int res = Dice::roll(1, 100);
		result << "进行San Check: " << endl << "1d100=" << res << "|" << sanity << ", ";
		string rs  = Dice::check_result(res, sanity, rate);
		int	sum = -1;
		regex  ss("成功");
		smatch m_ss;
		string d1("");
		if (rs == "大成功") {
			result << rs << ", " << endl;
			d1 = dices[0];
			if (d1.find("+")) {
				vector<string> ds = split(d1, '+');
				int	dx = stoi(ds[1]);
				result << "San值减少(1+" << dx << ")=";
				sum = dx + 1;
			} else if (d1.find("d"))
				sum = 1;
			else
				sum = stoi(d1);
		} else if (regex_search(rs, m_ss, ss)) {
			result << "成功, " << endl << "San值减少";
			d1 = dices[0];
		} else if (rs == "大失败") {
			result << rs << ", " << endl << "San值减少";
			d1 = dices[1];
		} else {
			result << "失败, " << endl << "San值减少";
			d1 = dices[1];
		}

		regex  gr("大"), plus("\\+"), dd("d");
		smatch m_gr, m_p, m_dd;
		if (regex_search(rs, m_gr, gr) && sum == -1) {
			if (regex_search(d1, m_p, plus)) {
				vector<string> ds = split(d1, '+');
				string dc = ds[0];
				int	dx = stoi(ds[1]), d = stoi(extract(dc, 1, 'd'));
				result << "(" << d << "+" << dx << ")=";
				sum = dx + d;
			} else if (regex_search(d1, m_dd, dd)) {
				sum = stoi(extract(d1, 1, 'd'));
			} else
				sum = stoi(d1);
		} else if (sum == -1) {
			if (regex_search(d1, m_p, plus)) {
				vector<string> ds   = split(d1, '+');
				int	dx = stoi(ds[1]);
				string di = ds[0];
				vector<string> temp = split(di, 'd');
				int n = stoi(temp[0]);
				int d = stoi(temp[1]);
				result << n << "d" << d << "+" << dx << "=";
				int f = Dice::roll(n, d);
				result << "(" << f << "+" << dx << ")=";
				sum = f + dx;
			} else if (regex_search(d1, m_dd, dd)) {
				vector<string> temp = split(d1, 'd');
				int	n = stoi(temp[0]);
				int	d = stoi(temp[1]);
				result << n << "d" << d << "=";
				sum = Dice::roll(n, d);
			} else
				sum = stoi(d1);
		}
		result << sum << ", " << endl;
		result << "当前San值为" << sanity - sum;
	}
	return result.str();
}
