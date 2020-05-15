#include <iostream>
#include <set>
#include <sstream>
#include <cmrc/cmrc.hpp>
#include <cqcppsdk/cqcppsdk.h>
#include <dice_maid/maid.h>
#include <math.h>

CMRC_DECLARE(app);

using namespace cq;
using namespace std;
using Message = cq::message::Message;
using MessageSegment = cq::message::MessageSegment;


string get_member_name(const int64_t group_id, const int64_t user_id) {
    GroupMember member = get_group_member_info(group_id, user_id);
    if (member.card != "") return member.card;
    return member.nickname;
}

GroupRole get_member_role(const int64_t group_id, const int64_t user_id) {
    GroupMember member = get_group_member_info(group_id, user_id);
    return member.role;
}

CQ_INIT {
    on_enable([] { 
        string path = get_data_path();
        if (path != "" && _access(path.c_str(), 0) == -1) CreateDirectory(path.c_str(), NULL);
        Maid::init();
        logging::info("enable", "plugin has been enabled");
    });

    on_private_message([](const PrivateMessageEvent &e) {
        try {
            /*
            auto msgid = send_private_message(e.user_id, e.message); // sent the incoming message back
            logging::info_success("private", "private message  , messeage Id: " + to_string(msgid));
            send_message(e.target,
                         MessageSegment::face(111) + "message constructed by Message module");
            */
            string msg = string(e.message);
            stringstream response;
            try {
                response << Maid::command(e.user_id, msg);
            } catch (const exception& exc) {
                stringstream result;
                result << "internal error: " << exc.what() << "\noriginal message: " << msg;
                logging::error("private", result.str());
            }
            if (response.str() != "") {
                send_private_message(e.user_id, Message(response.str()));
            }
        } catch (ApiError &err) {
            logging::error("private", "private message sending error, error code: " + to_string(err.code));
        }
    });
    /*
    on_message([](const MessageEvent &e) {
        logging::debug("message", "received: " + e.message + "\ntype: " + typeid(e).name());
    });
    */

    on_group_message([](const GroupMessageEvent &e) {
        /*
        static const set<int64_t> ENABLED_GROUPS = {123456, 123457};
        if (ENABLED_GROUPS.count(e.group_id) == 0) return; // ignore unabled group

        try {
            send_message(e.target, e.message);
            auto mem_list = get_group_member_list(e.group_id); // get member list
            string msg;
            for (auto i = 0; i < min(10, static_cast<int>(mem_list.size())); i++) {
                msg += "nickname: " + mem_list[i].nickname + "\n";
            }
            send_group_message(e.group_id, msg);
        } catch (ApiError& err) {
            logging::error("group", "group message sending error, error code: " + to_string(err.code));
        }
        if (e.is_anonymous()) {
            logging::info("group", "anonymous, nickname: " + e.anonymous.name);
        }
        e.block(); // block the message to next plugin
        */
        string msg = string(e.message);
        string last_msg = "";
        int count = 0;
        int multiples = 1000;
        bool repeat = false;
        bool repeat_switch = false;
        regex hidden("^#h"), dissmiss("^#!dissmiss\\s\\d+$"), jrrp("^#jrrp$"), set("^#set.*");
        smatch m_h, m_diss, m_jrrp, m_set;
        try {
            if (regex_match(msg, m_diss, dissmiss) && stoi(extract(msg, 1, ' ')) == get_login_user_id()) {
                Message message("");
                if (get_member_role(e.group_id, e.user_id) == cq::GroupRole::MEMBER) {
                    message += "premission denied";
                    send_group_message(e.group_id, message);
                } else {
                    message += "leave the group";
                    send_group_message(e.group_id, message);
                    set_group_leave(e.group_id, false);
                }
            } else {
                string response;
                string name = get_member_name(e.group_id, e.user_id);
                int64_t id = (regex_match(msg, m_jrrp, jrrp)) ? e.user_id : e.group_id;
                //复读次数
                if (msg.compare(last_msg)) {
                    count = 0;
                    repeat = false;
                }
                else count++;
                if (regex_match(msg, m_set, set)
                && get_member_role(e.group_id, e.user_id) == GroupRole::MEMBER) {
                    Message message("premission denied");
                    send_group_message(e.group_id, message);
                } else {
                    response = Maid::command(id, name, msg);
                    if (msg != "#help" && regex_search(msg, m_h, hidden) && response != "") {
                        Message pri(response), pub(name);
                        pub += u8"roll a hidden dice";
                        send_private_message(e.user_id, pri);
                        send_group_message(e.group_id, pub);
                    } else if (response != "") {
                        Message message(response);
                        send_group_message(e.group_id, message);
                    } else if(repeat_switch) {
                        if (common::random(multiples) - multiples * tanh(0.2*count) < 0 && !repeat){
                            send_group_message(e.group_id, msg);
                            repeat = true;
                        }
                    }
                    if(response.find("repeat") != string::npos) {
                        repeat_switch = (response.find("on") == string::npos)? true : false;
                    }
                }
                last_msg = msg;
            }
        } catch (const logic_error& exc) {
            stringstream result;
            result << "internal logic error: " << exc.what() << "\noriginal message: " << msg;
            logging::error("group", result.str());
        } catch (ApiError &err) {
            logging::error("group", "group message sending error, error code: " + to_string(err.code));
        } catch (const exception &exc) {
            stringstream result;
            result << "internal error: " << exc.what() << "\noriginal message: " << msg;
            logging::error("group", result.str());
        }
    });
    /*
    on_group_upload([](const auto &e) {
        stringstream ss;
        ss << "uploading a file, file name: " << e.file.name << ", size(byte): " << e.file.size;
        try {
            send_message(e.target, ss.str());
        } catch (ApiError &) {
        }
    });
    */
}
/*
CQ_MENU(menu_demo_1) {
    logging::info("menu", "clicked menu1");
}

CQ_MENU(menu_demo_2) {
    send_private_message(10000, "testing");
}
*/
