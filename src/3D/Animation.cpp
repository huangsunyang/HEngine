#include "3D/Animation.hpp"
#include "utils/StringUtils.hpp"
#include "utils/Parser.hpp"
#include <fstream>
#include "LogManager.hpp"

using namespace Utils;

void Animation::loadFromFile(string filename) {
    std::fstream f(filename, std::ios::in);
    auto p = Parser(f);
    string token;
    while (p.parse(token)) {
        if (token == "range") {
            float mi, ma;
            p.parse(mi, ma);
        } else if (token == "channel") {
            Channel channel;
            p.skip(2);
            p.parse(channel.m_extrapolate_l, channel.m_extrapolate_r);
            
            p.skip(1);
            int num;
            p.parse(num);
            p.skip(1);
            for (int i = 0; i < num; i++) {
                KeyFrame k;
                p.parse(k.m_time, k.m_value, k.m_interpolate_l, k.m_interpolate_r);
                channel.m_frames.push_back(k);
            }
            p.skip(2);
            m_channels.push_back(channel);
        } 
    }
    INFO("----------------- channel size %d\n", m_channels.size());
}

shared_ptr<Pose> Animation::getPose(float time) {
    auto pose = std::make_shared<Pose>();
    for (auto& channel: m_channels) {
        pose->m_dofs.push_back(channel.getValue(time));
    }
    return pose;
}