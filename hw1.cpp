#include <iostream>
#include <ctime>
using namespace std;

void getPlan(string birth, string today);
void ShowDate(tm time);
// void ShowDate(const tm *time);

int main()
{
    string birth, today; // 出生日期，今天的日期

    cout << "你好，这里是【生日聚会计划便签】" << endl;
    system("pause");

    // 输入出生日期和今天的日期
    cout << "请输入出生日期（例如：20240227）：" << endl;
    cin >> birth;
    cout << "请输入今天的日期：" << endl;
    cin >> today;

    getPlan(birth, today);

    system("pause");
    return 0;
}

void getPlan(string birth, string today)
{
    int x, n;      // 差距天数
    bool loop = 1; // 是否重新确定计划日期

    // 准备转换成int类型
    string year, month, day;
    // 这玩意儿是个傻的，不赋值会出问题
    tm birth_time={}, today_time={};

    // 按照年月日分段
    year = birth.substr(0, 4);
    month = birth.substr(4, 2);
    day = birth.substr(6, 2);
    // 利用atoi转换成int类型
    birth_time.tm_year = atoi(year.c_str()) - 1900;
    birth_time.tm_mon = atoi(month.c_str()) - 1;
    birth_time.tm_mday = atoi(day.c_str());

    // 故技重施
    year = today.substr(0, 4);
    month = today.substr(4, 2);
    day = today.substr(6, 2);
    today_time.tm_year = atoi(year.c_str()) - 1900;
    today_time.tm_mon = atoi(month.c_str()) - 1;
    today_time.tm_mday = atoi(day.c_str());

    // cout << birth_time.tm_year + 1900 << "." << birth_time.tm_mon + 1 << "." << birth_time.tm_mday << endl;
    // cout << today_time.tm_year + 1900 << "." << today_time.tm_mon + 1 << "." << today_time.tm_mday << endl;

    // 判断下次生日在今年还是明年
    birth_time.tm_year = today_time.tm_year;
    if ((birth_time.tm_mon < today_time.tm_mon) || (birth_time.tm_mon == today_time.tm_mon && birth_time.tm_mday <= today_time.tm_mday))
    {
        birth_time.tm_year++;
    }

    // 计算时间差
    time_t birth_times = mktime(&birth_time);
    time_t today_times = mktime(&today_time);

    x = difftime(birth_times, today_times) / 86400;

    // cout << "birth_times:" << birth_times << endl;
    // cout << "today_times:" << today_times << endl;

    cout << "今天到下次生日还有" << x << "天" << endl;
    cout << "-----准备为生日聚会计划制定日期-----" << endl;
    system("pause");

    while (loop)
    {
        cout << "下次生日日期：";
        ShowDate(birth_time);
        cout << "距离今天的天数：" << x << "天" << endl;
        cout << "请问您希望提前多少天做聚会计划？" << endl;
        cin >> n;

        // 计算初步计划日期
        time_t ad_times = birth_times - n * 86400;
        // tm *ad_time = localtime(&ad_times);
        tm *time = localtime(&ad_times);
        tm ad_time = *time;
        cout << "计划日期为：";
        ShowDate(ad_time);

        // 调整计划日期
        int week = ad_time.tm_wday;
        time_t plan_times = ad_times;
        tm plan_time;

        // 如果是周一、周二则调到前一个的周六
        if (week == 1 || week == 2)
        {
            cout << "计划日期为【工作日-周" << week << "】，调整到前一个周六" << endl;
            plan_times -= (week * 86400);
        }
        // 如果是周三、周四、周五则调到后一个周六
        else if (week == 3 || week == 4 || week == 5)
        {
            cout << "计划日期为【工作日-周" << week << "】，调整到后一个周六" << endl;
            plan_times += ((6 - week) * 86400);
        }
        else
        {
            cout << "计划日期为【周" << week << "】，无需调整" << endl;
        }

        // 这里有坑，用断点找到地方之后百思不得其解,琢磨良久后放弃，在网上找到了解答：
        /*连续使用localtime()的结果就是返回相同的时间，不会得到你想要的两个不同的时间。
          localtime()返回id指针会指向同一个区域，所以你不管调几次都是返回该指针指向的区域，不会变的。*/
        // tm *plan_time = localtime(&plan_times);

        time = localtime(&plan_times);
        plan_time = *time;

        // cout << "ad_times:" << ad_times << endl;
        // cout << "plan_times:" << plan_times << endl;

        cout << "计划日期为：";
        ShowDate(plan_time);
        system("pause");

        cout << "--------【结果显示】--------" << endl;
        cout << "下次生日日期：";
        ShowDate(birth_time);
        cout << "距离今天的天数：" << x << "天" << endl;
        cout << "距离下次生日前" << n << "天的日期：";
        ShowDate(ad_time);
        cout << "计划日期为：";
        ShowDate(plan_time);

        // 询问是否需要重新确定计划
        cout << "是否重新确定计划日期？(Y/N)" << endl;
        char choice;
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
        {
            loop = 1;
            cout << "-----重新为生日聚会计划制定日期-----" << endl;
        }
        else
        {
            loop = 0;
            cout << "------生日聚会计划完成------" << endl;
            cout << "最终制定生日计划的日期为：";
            ShowDate(plan_time);
        }
    }
}

// 输出日期函数
void ShowDate(const tm time)
{
    cout << time.tm_year + 1900 << "." << time.tm_mon + 1 << "." << time.tm_mday << endl;
}
// void ShowDate(const tm *time)
// {
//     cout << time->tm_year + 1900 << "." << time->tm_mon + 1 << "." << time->tm_mday << endl;
// }