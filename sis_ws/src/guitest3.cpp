#include <conio.h>
#include <windows.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <easyx.h>
#include <vector>
#include <thread>
#include <sstream>


class Botton{  //按钮控件
private:
    int left = 0, top = 0, right = 0, bottom = 0, ellipsewidth = 0, ellipseheight = 0;
    LPCTSTR defaultText;
    int defaultTextSize;
    int mouseStyle = 0; //0代表正常箭头鼠标，1代表手型鼠标
    LPCTSTR defaultTextFont;
    COLORREF defaultTextColor;
    COLORREF defaultBottonColor;
    COLORREF mouseInBottonColor;
public:

    void Draw(int topLeft_x, int topLeft_y, int downRight_x, int downRight_y, int el_width, int el_height){  //绘制按钮
        left = topLeft_x;
        top = topLeft_y;
        right = downRight_x;
        bottom = downRight_y;
        ellipsewidth = el_width;
        ellipseheight = el_height;
        solidroundrect(left,top,right,bottom,ellipsewidth,ellipseheight);
    }

    void setColor(COLORREF theColor, COLORREF mouseInColor){  //设置按钮默认颜色（无操作时的颜色，鼠标悬停于按钮上的颜色）
        defaultBottonColor = theColor;
        mouseInBottonColor = mouseInColor;
        setbkcolor(theColor);
        setfillcolor(theColor);
    }

    void setDefaultText(LPCTSTR defaultStr, int textSize, LPCTSTR textFont, COLORREF textColor){  //设置按钮默认文本
        defaultText = defaultStr;
        defaultTextSize = textSize;
        defaultTextFont = textFont;
        defaultTextColor = textColor;
        RECT r = { left, top, right, bottom };
        settextcolor(textColor);
        settextstyle(textSize, 0, _T(textFont));
        drawtext(_T(defaultText), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    void changeWhileMouseInBox(int x,int y){  //鼠标悬停时按钮变化，改变按钮颜色及鼠标样式
        HWND hwnd = GetHWnd();
        if (Check(x,y)){
            if (mouseStyle == 0){
                changeBottonColor(mouseInBottonColor);
                HCURSOR hcur = LoadCursor(NULL, IDC_HAND);  //加载手型鼠标样式
                SetClassLongPtrA(hwnd, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(hcur)); //应用更改
                mouseStyle = 1;
            }
        }
        else{  //当鼠标离开按钮时，恢复其样式
            if (mouseStyle == 1){
                changeBottonColor(defaultBottonColor);
                HCURSOR hcur = LoadCursor(NULL, IDC_ARROW);  //加载箭头鼠标样式
                SetClassLongPtrA(hwnd, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(hcur)); //还原更改
                mouseStyle = 0;
            }
        }
    }

    bool Check(int x, int y)  //检验函数，用于页面发生单击事件时，检验鼠标是否在控件中
    {
        bool mouseInBlock = left <= x && x <= right && top <= y && y <= bottom;
        return (mouseInBlock);
    }

    void changeBottonColor(COLORREF theColor){  //改变按钮颜色
        setbkcolor(0);
        setfillcolor(theColor);
        solidroundrect(left,top,right,bottom,ellipsewidth,ellipseheight);
        setbkcolor(theColor);  //重绘按钮及按钮文本
        setDefaultText(defaultText,defaultTextSize,defaultTextFont,defaultTextColor);
    }
};

class TextBox{ //文本框控件
private:
    int left = 0, right = 0, bottom = 0;  //文本框横线左端点x坐标，右端点x坐标，横线底部y坐标
    int textLen = 0;
    int enter_x;
    int mouseStyle = 0; //0代表正常箭头鼠标，1代表I型鼠标
    bool typing = true;
    LPCTSTR defaultText;
    int defaultTextSize;
    LPCTSTR defaultTextFont;
    COLORREF defaultTextColor;
    std::vector<char> textInBox;

public:
    void Draw(int left_x, int right_x, int line_y){  //绘制文本框
        left = left_x;
        enter_x = left;
        right = right_x;
        bottom = line_y;
        setlinecolor(BLACK);
        setlinestyle(PS_SOLID,2);
        line(left,bottom,right,bottom);
    }

    void setBackColor(COLORREF theColor){  //设置文本框底部横线默认颜色
        setbkcolor(theColor);
    }

    void setDefaultText(LPCTSTR defaultStr, int textSize, LPCTSTR textFont, COLORREF textColor){  //设置文本框无输入默认文本
        defaultText = defaultStr;
        defaultTextSize = textSize;
        defaultTextFont = textFont;
        defaultTextColor = textColor;
        RECT r = { left , bottom - 26, right, bottom };
        settextcolor(textColor);
        settextstyle(textSize, 0, _T(textFont));
        drawtext(_T(defaultText), &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    void initialize(int x, int y){  //初始化文本框，开启三个同时进行的线程
        if (!Check(x,y)){
            typing = false;
        }
        std::thread t1([this]{cursorBlinking();});  //光标闪烁进程
        std::thread t2([this]{Type();});  //等待输入进程
        std::thread t3([this]{mouseInBoxCheck();});  //鼠标位置检测进程
        t1.join();
        t2.join();
        t3.join();
        std::cout << "Init ended" << std::endl;
        if (textLen == 0){
            setDefaultText(defaultText,defaultTextSize,defaultTextFont,defaultTextColor);
        }
    }

    void mouseInBoxCheck(){  //循环检测鼠标是否在文本框中
        ExMessage mouseMonitor{};
        int x = mouseMonitor.x;
        int y = mouseMonitor.y;
        while(typing){
            mouseMonitor =  getmessage(EX_MOUSE);
            if (mouseMonitor.message == WM_LBUTTONDOWN && !Check(x,y)){  //若在文本框外按下左键，终止输入状态
                typing = false;
                std::cout << "Typing stop" << std::endl;
                keybd_event(VK_RETURN,0,0,0); //按下enter，使Type()跳出_getch()等待状态
                break;
            }
        }
        std::cout << "InBoxCheck ended" << std::endl;
    }

    void changeWhileMouseInBox(int x,int y){  //改变鼠标光标
        HWND hwnd = GetHWnd();
        if (Check(x,y)){
            if (mouseStyle == 0){
                HCURSOR hcur = LoadCursor(NULL, IDC_IBEAM);  //加载I型鼠标样式
                SetClassLongPtrA(hwnd, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(hcur)); //应用更改
                mouseStyle = 1;
            }
        }
        else{  //若鼠标在文本框外，还原光标
            if (mouseStyle == 1){
                HCURSOR hcur = LoadCursor(NULL, IDC_ARROW);  //加载I型鼠标样式
                SetClassLongPtrA(hwnd, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(hcur)); //还原更改
                mouseStyle = 0;
            }
        }
    }

    bool Check(int x, int y)  //检验函数，用于页面发生单击事件时，检验鼠标是否在控件中
    {
        bool mouseInBlock = left <= x && x <= right && bottom - 40 <= y && y <= bottom;
        if (mouseInBlock){
            typing = true;
        }
        else{
            typing = false;
        }
        return (mouseInBlock);
    }

    void changeLineColor(COLORREF lineColor){  //设置输入状态时，文本框底部横线的颜色
        setlinestyle(PS_SOLID,2);
        setlinecolor(lineColor);
        line(left,bottom,right,bottom);
    }

    void cursorBlinking(){  //光标闪烁函数
        int curCursorPos = enter_x;
        while (typing){
            if (curCursorPos != enter_x){
                curCursorPos = enter_x;
            }
            setlinestyle(PS_SOLID,2);
            setlinecolor(RGB(21,85,154));
            line(curCursorPos,bottom-25,curCursorPos,bottom-3);
            usleep(500000);  //等待半秒
            setlinecolor(RGB(185,222,201));
            line(curCursorPos,bottom-25,curCursorPos,bottom-3);
            usleep(500000);
        }
        std::cout << "cursor ended" << std::endl;
    }

    void Type(){  //输入检测函数
        changeLineColor(RGB(21,85,154));
        if (textLen == 0){
            setbkcolor(RGB(185,222,201));
            clearrectangle(left , bottom - 26, right, bottom - 2);
        }
        settextcolor(RGB(50,50,50));
        settextstyle(24, 0, "Arial");
        if (!enter_x){
            enter_x = left;  //初始化输入坐标
        }
        while(typing){
            int ch = _getch(); //得到输入按键的ASCII码
            char char_ch =(char)ch; //将ASCII码转成char型
            switch(ch){
                case '\b':  //输入退格时，删除一位数
                    if (textLen > 0){
                        settextcolor(RGB(185,222,201));
                        char del_letter = textInBox.back();  //从vector中取出最后一位数字
                        std::cout << del_letter << std:: endl;
                        enter_x -= textwidth(del_letter) + 1;  //计算最后一位数字的坐标
                        outtextxy(enter_x,bottom-25,del_letter);  //重绘数字，颜色于背景色相同，达到删除效果
                        std::cout << enter_x << bottom - 25 << std:: endl;
                        settextcolor(RGB(50,50,50));
                        textInBox.pop_back();  //从vector中删除此位数字
                        textLen -= 1;
                    }
                    break;
                case '\r':
                case '\n':  //键盘输入enter时，结束输入状态
                    typing = false;
                    changeLineColor(BLACK);  //恢复文本框颜色为黑色
                    break;
                default:  //正常输入状态
                    if (textLen < 23){
                        std::cout << "typing" << char_ch << std::endl;
                        textInBox.push_back(char_ch);  //将字符加入到vector中
                        textLen += 1;
                        outtextxy(enter_x,bottom-25,char_ch);  //绘制数字
                        enter_x += textwidth(char_ch) + 1;  //更新输入位置坐标
                    }
            }
        }
        std::cout << "type ended" << std::endl;
    }

    std::string value(){  //输出文本框中的字符
        std::stringstream ss;
        std::string result;
        for (char letter: textInBox){
            ss << letter;
        }
        result = ss.str();
        return result;
    }
};
void page2();
void startAnimation(){  //启动时ISIS的飞入效果
    std::string title = "Integrated Student Information System";
    settextcolor(WHITE);
    settextstyle(100, 53, "Arial Rounded MT Bold");
    int deltaDistance = 30;  //每次移动的距离
    int title_targetPos_x = 120;
    int title_charPos_y = 120;
    for (int i = 0; i < title.length(); i++){
        if (title[i] != ' '){
            int title_charPos_x = title_targetPos_x+150;
            outtextxy(title_charPos_x, title_charPos_y, title[i]);  //起始位置先绘制一次
            while (title_charPos_x > title_targetPos_x){  //当未到目标位置时，持续移动字符
                usleep(1000); //挂起1000us
                settextcolor(RGB(154,190,175));
                outtextxy(title_charPos_x, title_charPos_y, title[i]);  //用背景色绘制字符，达到删除效果
                title_charPos_x -= deltaDistance;  //更新x坐标
                settextcolor(WHITE);
                outtextxy(title_charPos_x, title_charPos_y, title[i]);  //在新位置绘制字符
            }
            title_targetPos_x += textwidth(title[i]) - 15;
        }
        else{  //遇到空格换行
            title_charPos_y += 100;
            title_targetPos_x = 120;
        }
    }
}

void logInZone(){  //登录区域绘制
    int changeRate = 50;  //动画细分率
    double deltaR = (185.0 - 154.0)/changeRate;  //计算delta用于动画的淡入效果
    double deltaG = (222.0 - 190.0)/changeRate;
    double deltaB = (201.0 - 175.0)/changeRate;
    double curR = 154;
    double curG = 190;
    double curB = 175;
    int curTop = 150;
    int curBottom = 600;
    setfillcolor(RGB(curR,curG,curB));
    for (int i = 0; i < changeRate; i++){ //淡入动画的循环
        usleep(1000);
        curR += deltaR;  //更新位置
        curG += deltaG;
        curB += deltaB;
        setfillcolor(RGB(curR,curG,curB));  //更新颜色的rgb值
        curTop -= 1;
        curBottom -= 1;
        solidroundrect(750,curTop,1150,curBottom,15,15);
    }
    Botton logInBotton;  //设置属性，创建各个控件
    logInBotton.setColor(RGB(86,152,195),RGB(22,97,171));
    logInBotton.Draw(800,430,1100,480,15,15);  //（左上角x，y坐标，右下角x，y坐标，圆角宽度，圆角长度）
    logInBotton.setDefaultText("Log in", 35, "Arial Rounded MT Bold", WHITE);
    TextBox usernameBox;
    usernameBox.setBackColor(RGB(185,222,201));
    usernameBox.Draw(800,1100,250);  //（左端点x坐标，右端点x坐标，横线底部y坐标）
    usernameBox.setDefaultText("Username",24,"Arial",RGB(80,80,80));
    TextBox passwordBox;
    passwordBox.setBackColor(RGB(185,222,201));
    passwordBox.Draw(800,1100,350);
    passwordBox.setDefaultText("Password",24,"Arial",RGB(80,80,80));
    ExMessage mouseMonitor{};
    while(true){  //鼠标事件监测循环
        mouseMonitor = getmessage(EX_MOUSE);
        std::cout << "Waiting for mouseclick" << std::endl;
        int x, y;
        x = mouseMonitor.x;
        y = mouseMonitor.y;
        usernameBox.changeWhileMouseInBox(x,y);  //启用各个控件的鼠标样式改变函数，达到在某控件触发时改变鼠标样式的效果
        passwordBox.changeWhileMouseInBox(x,y);
        logInBotton.changeWhileMouseInBox(x,y);
        if (mouseMonitor.message == WM_LBUTTONDOWN){  //左键按下时，三个if监测鼠标按了哪个控件
            if(usernameBox.Check(x,y)){
                usernameBox.initialize(x,y);  //初始化文本框，开启各线程
                std::cout << usernameBox.value() << std::endl;
            }
            if(passwordBox.Check(x,y)){
                passwordBox.initialize(x,y);
                std::cout << passwordBox.value() << std::endl;
            }
            if(logInBotton.Check(x,y)){  //当按钮按下时
                logInBotton.changeBottonColor(RGB(134,157,157));
                usleep(5000);
                page2();  //触发的事件，此处暂时为跳转第二页
                break;
            }
        }
    }

}

void page2Animation(){  //第二页出现的动画，原理于logInZone相同
    int changeRate = 120;
    double deltaR = (134.0 - 185.0)/changeRate;
    double deltaG = (157.0 - 222.0)/changeRate;
    double deltaB = (157.0 - 201.0)/changeRate;
    double curR = 185;
    double curG = 222;
    double curB = 201;
    int curTop = -720;
    int curBottom = 0;
    setfillcolor(RGB(curR,curG,curB));
    for (int i = 0; i < changeRate; i++){
        usleep(1000);
        curR += deltaR;
        curG += deltaG;
        curB += deltaB;
        setfillcolor(RGB(curR,curG,curB));
        curTop += 6;
        curBottom += 6;
        solidrectangle(0,curTop,1280,curBottom);
    }
    RECT r = { 80,150,400,300 };
    settextcolor(WHITE);
    settextstyle(60, 0, _T("Arial Rounded MT Bold"));
    drawtext(_T("Welcome!"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void page2(){  //第二页的主函数
    page2Animation();
    Botton logInBotton;  //创建一个按钮（测试用）
    logInBotton.setColor(RGB(86,152,195),RGB(22,97,171));
    logInBotton.Draw(300,300,980,420,15,15);
    logInBotton.setDefaultText("HELLO", 80, "Arial Rounded MT Bold", WHITE);
    ExMessage mouseMonitor{};
    while(true) {  //第二页的鼠标事件监测
        mouseMonitor = getmessage(EX_MOUSE);
        std::cout << "Waiting for mouseclick" << std::endl;
        int x, y;
        x = mouseMonitor.x;
        y = mouseMonitor.y;
        logInBotton.changeWhileMouseInBox(x,y);  //启用控件的鼠标样式改变函数，达到在某控件触发时改变鼠标样式的效果
        if (mouseMonitor.message == WM_LBUTTONDOWN) {
            if (logInBotton.Check(x, y)) {
                logInBotton.changeBottonColor(RGB(134, 157, 157));
                usleep(5000);  //这里写触发的事件，但还没写。。
                break;
            }
        }
    }
}

int main()
{
    initgraph(1280, 720);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int x = (screenWidth - 1280)/2;
    int y = (screenHeight - 720)/2;  //计算屏幕坐标，使窗口居中
    HWND hwnd = GetHWnd();
    SetWindowPos(hwnd, NULL, x, y, 1280, 720, SWP_SHOWWINDOW);
    setbkcolor(RGB(154,190,175));  //设置背景色
    cleardevice();  //用背景色清空屏幕
    settextcolor(BLACK);
    startAnimation();  //唤起字体动画
    logInZone();  //绘制登录区域
    _getch();
    closegraph();

    return 0;
}
