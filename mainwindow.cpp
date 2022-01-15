#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textBrowser->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_cmdLineEdit_editingFinished()
{
    int lineNum = 0;
    bool ifNeedInsert = true;
    QString cmd = ui->cmdLineEdit->text();
    if(cmd == "")
        return;
    string s = string((const char *)cmd.toLocal8Bit());
    if(s[0] == '?'){
        cout<<"s:"<< s <<endl;
        Tokenizer scanner(s.substr(1));
        string varString = scanner.ReadToken();
        cout<<varString<<endl;
        int var = 0;
        try{
            var = stoi(varString);
            cout<<var<<endl;
            program.getInput(var);
            ui->cmdLineEdit->setText("");
            return;

        }
        catch(std::invalid_argument){ //catch之后处理的方法还不怎么清楚
            ifNeedInsert = false;
            cout<<"Invalid Var!"<<endl;
        }
    }
    ui->cmdLineEdit->setText("");
    int indexNumber = s.find(" ");
    if (indexNumber != -1){
        try{
            string NumString = s.substr(0,indexNumber);
            lineNum = stoi(NumString);
        }
        catch(std::invalid_argument){
            ifNeedInsert = false;
        }

        if(lineNum >= 0 && ifNeedInsert == true){
            string line = s.substr(indexNumber+1);
            program.addNewLine(lineNum,line);
            cout<< lineNum << " "<<s.substr(indexNumber+1)<<endl;
        }
    }
    //如果不是的话直接开始control的处理
    if(indexNumber == -1 || ifNeedInsert == false){
        //case1:全为数字，相当于将相应行号的line删除
        int i = 0;
        for(i = 0;i < s.length();++i){
            if(!((s[i] >= '0')&&(s[i] <= '9')))
                break;
        }
        if(i == s.length()){
            lineNum = stoi(s);
            if(program.lineStore.count(lineNum) == 1)
                program.removeLine(lineNum);
        }
        else{
            Tokenizer scanner(s);
            string controlCmd = scanner.ReadToken();

            //处理6种command
            int j = 0;
            for(;j < 6;++j){
                if(Control[j] == controlCmd)
                    break;
            }
            switch (j) {
                case 0:on_btnRunCode_clicked();
                       break;
                case 1:on_btnLoadCode_clicked();
                       break;
                case 2:ListCode();
                       break;
                case 3:on_btnClearCode_clicked();
                       break;
                case 4:Showhelp();
                       break;
                case 5:this->close();
                       break;               
            }

            //处理3种可能立即执行的语句
            int k = 0;
            for(;k < 3;++k){
                if(InstantState[k] == controlCmd)
                    break;
            }
            Statement *nowState;
            string outString;
            QString output;
            switch (k) {
                case 0:program.getInstantParsed(s);
                       nowState = program.getState();
                       nowState->Handling(evalTable,program);
                       break;
                case 1:program.getInstantParsed(s);
                       nowState = program.getState();
                       nowState->Handling(evalTable,program);
                       outString = program.getOutput();
                       output = QString(QString::fromLocal8Bit(outString.c_str()));
                       ui->textBrowser->append(output);
                       ui->textBrowser->repaint();
                       break;
                case 2:lastIdenti = scanner.ReadToken();
                       handleInput(lastIdenti);
                       program.getInstantParsed(s);
                       nowState = program.getState();
                       nowState->Handling(evalTable,program);
                       break;
                default:throw error("Unknown command or Statement!");
                       break;
            }
        }
    }

    //在代码区域内显示
    ui->CodeDisplay->clear();
    map<int ,string>::iterator itr;
    for(itr = program.lineStore.begin();itr != program.lineStore.end();itr++){
        string tmpString = program.displayMap(itr->first);
        cmd = QString(QString::fromLocal8Bit(tmpString.c_str()));
        ui->CodeDisplay->append(cmd);
    }
    return;

}

void MainWindow::GetInput()
{
}

void MainWindow::ChangeValue()
{
}

void MainWindow::handleInput(string s)
{
    cout << s <<endl;
    ui->cmdLineEdit->clear();
    ui->cmdLineEdit->setText("?");
    QElapsedTimer time;
    time.start();
    while(time.elapsed() <1000)             //等待时间流逝1秒钟
        QApplication::processEvents();
    ui->cmdLineEdit->clear();
    time.start();
    while(time.elapsed() <3000)             //等待时间流逝3秒钟
        QApplication::processEvents();
    cout<<"get input1"<<endl;
    QString tmp =  ui->cmdLineEdit->text();
    string str = string((const char *)tmp.toLocal8Bit());
    if(str.empty())
        cout << "empty!" << endl;
    cout <<str<<endl;
    int var = stoi(str);
    cout << "var:" << var << endl;
    program.getInput(var);
    ui->cmdLineEdit->setText("");

}

void MainWindow::EditClear()
{
    ui->cmdLineEdit->clear();
}

void MainWindow::on_btnClearCode_clicked()
{
    ui->CodeDisplay->clear();
    ui->treeDisplay->clear();
    ui->textBrowser->clear();
    program.clear();
}

void MainWindow::on_btnLoadCode_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("open a file."),
            "D:/",
            tr("text files(*.txt *.doc *.docx)"));
        //如果没有打开任何文件
        if (fileName.isEmpty()) {
            QMessageBox::warning(this, "Warning!", "Failed to open the text file!");
        }
        else {
            try{
                string s = string((const char *)fileName.toLocal8Bit());
                ifstream infile(s);
                string line;
                if(!infile)
                    throw error("can't open the file!");

                while(getline(infile,line)){
                    int indexNumber = line.find(" ");
                    string NumString = line.substr(0,indexNumber);
                    int lineNum = stoi(NumString);
                    string newline = line.substr(indexNumber+1);
                    program.addNewLine(lineNum,newline);
                }
                infile.close();

                ui->CodeDisplay->clear();
                map<int ,string>::iterator itr;
                //按序排列显示
                for(itr = program.lineStore.begin();itr != program.lineStore.end();itr++){
                    string tmpString = program.displayMap(itr->first);
                    QString cmd = QString(QString::fromLocal8Bit(tmpString.c_str()));
                    ui->CodeDisplay->append(cmd);
                }

            }catch(error LoadError){
                LoadError.printString();
                exit(0);
            }
        }

}


void MainWindow::on_btnRunCode_clicked()
{
    int i = 0;
    int nowLine = 1;
    map<int ,string>::iterator itr;
    itr = program.lineStore.begin();
    ui->textBrowser->clear();
    ui->treeDisplay->clear();
    //构建表达式树
    handleSyntaxTree();
    while(itr != program.lineStore.end()){
        try {
            //获取下一步要执行的行号
            if(program.isJmp() == true){
                nowLine = program.getNextLine();
                if(nowLine == -1)
                    break;
                for(itr = program.lineStore.begin();itr != program.lineStore.end();++itr){
                    if(itr->first == nowLine)
                        break;
                }
                if(itr == program.lineStore.end()){
                    throw error("jump to invalid line number!");
                }
                program.resetJmp();
            }
            else
                nowLine = itr->first;
            //处理 input 要再次输入的特殊情况,应该放在run函数里面
            Tokenizer reader(itr->second);
            string mark = reader.ReadToken();
            if(mark == "INPUT"){
                handleInput(reader.ReadToken());
                cout << "get here!"<<endl;
            }
            program.getParsedState(nowLine);
            Statement *nowState = program.getState();
            //print的情况需要特殊处理
            nowState->Handling(evalTable,program);
            if(mark == "PRINT"){
                string outString = program.getOutput();
                QString output = QString(QString::fromLocal8Bit(outString.c_str()));
                ui->textBrowser->append(output);
                ui->textBrowser->repaint();
            }
            ++i;
            ++itr;
        } catch (error Runerror) {
            cout << "runerror!" <<endl;
            string errorString = Runerror.printString();
            QString QerString = QString::fromStdString(errorString);
            QMessageBox::warning(this, tr("Error!"),QerString);
            this->on_btnClearCode_clicked();
            break;
        }
    }
}


void MainWindow::handleSyntaxTree()
{
    map<int ,string>::iterator itr;
    for(itr = program.lineStore.begin();itr != program.lineStore.end();itr++){
        try {
            //获取statement
            int nowLine = itr->first;
            program.getParsedState(nowLine);
            Statement *nowState = program.getState();
            //显示相应的语法树
            showSyntaxTree(nowState,nowLine);
        } catch (error TreeError) {
            //如果出错直接打印error
            QString output("error");
            ui->treeDisplay->append(output);
            ui->treeDisplay->repaint();
        }

    }
}

void MainWindow::showSyntaxTree(Statement *sta, int lineNum)
{
    Treenode *syntaxNode = sta->CreateTree(lineNum);
    Treenode *cur;
    string showString;
    int size = 0;
    vector<Treenode*> queue;
    //层次遍历语法树
    if(syntaxNode == nullptr)
        return;
    queue.push_back(syntaxNode);
    int depth = -1;
    while(!queue.empty()){
        size = queue.size();
        ++depth;
        for(int i = 0;i < size;++i){
            cur = queue.front();
            queue.erase(queue.begin());
            showString = string(depth * 4,' ') + cur->data;
            QString output = QString(QString::fromLocal8Bit(showString.c_str()));
            ui->treeDisplay->append(output);
            ui->treeDisplay->repaint();
            int csize = cur->children.size();
            for(int j = 0;j < csize;++j){
                queue.push_back(cur->children[j]);
            }
        }
    }
}

void MainWindow::Showhelp()
{
    QMessageBox::about(this, tr("Game Help"), tr("you should input your code in specific term and run"
        "<p>Space - pause & resume"));
}

void MainWindow::ListCode()
{
    ui->CodeDisplay->clear();
    map<int ,string>::iterator itr;
    for(itr = program.lineStore.begin();itr != program.lineStore.end();itr++){
        string tmpString = program.displayMap(itr->first);
        QString cmd = QString(QString::fromLocal8Bit(tmpString.c_str()));
        ui->CodeDisplay->append(cmd);
    }
    return;
}
