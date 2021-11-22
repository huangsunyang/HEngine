#pragma once

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include "Bone.hpp"

using std::string;
using std::shared_ptr;

struct KeyFrame {
    float m_time;
    float m_value;
    string m_interpolate_l;
    string m_interpolate_r;
};


struct Channel {
    float getValue(float time) {
        if (m_frames.empty()) {
            return 0.0f;
        } else if (m_frames[0].m_time > time) {
            return m_frames[0].m_value;
        } else if (m_frames.back().m_time < time) {
            return m_frames.back().m_value;
        } else {
            int i = 0;
            for (; i < m_frames.size(); i++) {
                if (time > m_frames[i].m_time) {
                    break;
                } 
            }
            auto time1 = m_frames[i].m_time, time2 = m_frames[i + 1].m_time;
            auto value1 = m_frames[i].m_value, value2 = m_frames[i + 1].m_value;
            return value1 + (value2 - value1) * (time - time1) / (time2 - time1);
        }
    }

    string m_extrapolate_l;
    string m_extrapolate_r;
    vector<KeyFrame> m_frames;  // in order
};


struct Pose {
    vector<float> m_dofs;
};


class Animation {
public:
    void loadFromFile(string filename);
    shared_ptr<Pose> getPose(float time);

protected:
    float m_length;
    vector<Channel> m_channels;
};