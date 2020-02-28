#include <cqcppsdk/cqcppsdk.h>
#include <dice_maid/maid.h>
#include <sstream>

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
        if (path != "" && _access(path.c_str(), 0) == -1) {
            if (! CreateDirectory(path.c_str(), NULL))
                throw - 1;
        } else if (path == "")
            throw - 1;
        Maid::init();
        logging::info("启用", "插件已启用");
    });

    on_private_message([](const PrivateMessageEvent &e) {
        try {
            /*
            auto msgid = send_private_message(e.user_id, e.message); // 直接复读消息
            logging::info_success("私聊", "私聊消息复读完成, 消息 Id: " + to_string(msgid));
            send_message(e.target,
                         MessageSegment::face(111) + "这是通过 message 模块构造的消息~"); // 使用 message 模块构造消息
            */
            string msg = string(e.message);
            stringstream response;
            try {
                response << Maid::command(e.user_id, msg);
            } catch (const exception &exc) {
                stringstream result;
                result << "内部错误: " << exc.what() << "\n原消息： " << msg;
                logging::error("私聊", result.str());
            }
            if (response.str() != "") {
                send_private_message(e.user_id, Message(response.str()));
            }
        } catch (ApiError &err) {
            logging::error("私聊", "私聊消息错误, 错误码: " + to_string(err.code));
        }
    });
    /*
    on_message([](const MessageEvent &e) {
        logging::debug("消息", "收到消息: " + e.message + "\n实际类型: " + typeid(e).name());
    });
    */

    on_group_message([](const GroupMessageEvent &e) {
        /*
        static const set<int64_t> ENABLED_GROUPS = {123456, 123457};
        if (ENABLED_GROUPS.count(e.group_id) == 0) return; // 不在启用的群中, 忽略

        try {
            send_message(e.target, e.message); // 复读
            auto mem_list = get_group_member_list(e.group_id); // 获取群成员列表
            string msg;
            for (auto i = 0; i < min(10, static_cast<int>(mem_list.size())); i++) {
                msg += "昵称: " + mem_list[i].nickname + "\n"; // 拼接前十个成员的昵称
            }
            send_group_message(e.group_id, msg); // 发送群消息
        } catch (ApiError& err) {
            logging::error("群聊", "群聊消息错误, 错误码: " + to_string(err.code));
        }
        if (e.is_anonymous()) {
            logging::info("群聊", "匿名消息, 昵称: " + e.anonymous.name);
        }
        e.block(); // 阻止当前事件传递到下一个插件
        */
        string msg = string(e.message);
        regex hidden("^#h"), dissmiss("^#!dissmiss\\s\\d+$"), jrrp("^#jrrp$"), set("^#set.*");
        smatch m_h, m_diss, m_jrrp, m_set;
        try {
            if (regex_match(msg, m_diss, dissmiss) && stoi(extract(msg, 1, ' ')) == get_login_user_id()) {
                Message message("");
                if (get_member_role(e.group_id, e.user_id) == cq::GroupRole::MEMBER) {
                    message += "权限不足, 请让群主或管理员操作";
                    send_group_message(e.group_id, message);
                } else {
                    message += "退出该群";
                    send_group_message(e.group_id, message);
                    set_group_leave(e.group_id, false);
                }
            } else {
                string response;
                string name = get_member_name(e.group_id, e.user_id);
                int64_t id = (regex_match(msg, m_jrrp, jrrp)) ? e.user_id : e.group_id;
                if (regex_match(msg, m_set, set) && get_member_role(e.group_id, e.user_id) == GroupRole::MEMBER) {
                    Message message("权限不足, 请让群主或管理员操作");
                    send_group_message(e.group_id, message);
                } else {
                    response = Maid::command(id, name, msg);
                    if (msg != "#help" && regex_search(msg, m_h, hidden) && response != "") {
                        Message pri(response), pub(name);
                        pub += u8"进行了一次暗骰";
                        send_private_message(e.user_id, pri);
                        send_group_message(e.group_id, pub);
                    } else if (response != "") {
                        Message message(response);
                        send_group_message(e.group_id, message);
                    }
                }
            }
        } catch (const logic_error &exc) {
            stringstream result;
            result << "内部逻辑错误: " << exc.what() << "\n原消息： " << msg;
            logging::error("群聊", result.str());
        } catch (ApiError &err) {
            logging::error("群聊", "群聊消息错误, 错误码: " + to_string(err.code));
        } catch (const exception &exc) {
            stringstream result;
            result << "内部错误: " << exc.what() << "\n原消息： " << msg;
            logging::error("群聊", result.str());
        }
    });
    /*
    on_group_upload([](const auto &e) { // 可以使用 auto 自动推断类型
        stringstream ss;
        ss << "您上传了一个文件, 文件名: " << e.file.name << ", 大小(字节): " << e.file.size;
        try {
            send_message(e.target, ss.str());
        } catch (ApiError &) {
        }
    });
    */
}
/*
CQ_MENU(menu_demo_1) {
    logging::info("菜单", "点击菜单1");
}

CQ_MENU(menu_demo_2) {
    send_private_message(10000, "测试");
}
*/
