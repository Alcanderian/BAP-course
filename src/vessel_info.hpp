#ifndef ALGOR_SHIP_PARKING_VESSEL_INFO_HPP
#define ALGOR_SHIP_PARKING_VESSEL_INFO_HPP

struct vessel_info {
    int index;
    int arri_time;
    int serv_time;
    int berth;

    vessel_info() {}

    vessel_info(int index, int arri_time, int serv_time, int berth)
            : index(index), arri_time(arri_time),
              serv_time(serv_time), berth(berth) {}

    vessel_info(const vessel_info &vi)
            : index(vi.index), arri_time(vi.arri_time),
              serv_time(vi.serv_time), berth(vi.berth) {}

    vessel_info &operator=(const vessel_info &vi) {
        this->index = vi.index;
        this->arri_time = vi.arri_time;
        this->serv_time = vi.serv_time;
        this->berth = vi.berth;

        return *this;
    }
};

bool operator<(const vessel_info &a, const vessel_info &b) {
    return a.index < b.index;
}

#endif //ALGOR_SHIP_PARKING_VESSEL_INFO_HPP
