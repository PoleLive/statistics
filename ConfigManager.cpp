#include "ConfigManager.h"

ConfigManager::ConfigManager():
    m_strTitle(""),
    m_strSubTitle("")
{

}

ConfigManager *ConfigManager::Instance()
{
    static ConfigManager s_Instance;
    return &s_Instance;
}

QList<QString> ConfigManager::ReadConfig()
{
    //打开或创建文件
    QFile file("config.xml"); //相对路径、绝对路径、资源路径都行
    if(!file.open(QFile::ReadOnly))
    {
        return QList<QString>();
    }

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return QList<QString>();
    }
    file.close();

    QDomElement root=doc.documentElement(); //返回根节点

    //获得第一个子节点
    QDomNode node=root.firstChild();

    QList<QString> PrintFieldList;

    //如果节点不空
    while(!node.isNull())
    {
        //如果节点是元素
        if(node.isElement())
        {
            //转换为元素，注意元素和节点是两个数据结构，其实差不多
            QDomElement e=node.toElement();
            if(e.nodeName() == "PrintFieldList")
            {
                QDomNodeList nodes = e.childNodes();
                for(int i = 0; i < nodes.count(); i++)
                {
                    QDomElement child=nodes.at(i).toElement();
                    PrintFieldList.append(child.toElement().text());
                    qDebug()<< child.toElement().text();
                }
            }
            else if(e.nodeName() == "PrintTitle")
            {
                QDomNodeList nodes = e.childNodes();
                for(int i = 0; i < nodes.count(); i++)
                {
                    QDomElement child=nodes.at(i).toElement();
                    if(child.nodeName() == "Title")
                    {
                        m_strTitle = child.toElement().text();
                    }
                    if(child.nodeName() == "Subtitle")
                    {
                        m_strSubTitle = child.toElement().text();
                    }
                }
            }
            else if(e.nodeName() == "Statistics")
            {
                QDomNodeList nodes = e.childNodes();
                for(int i = 0; i < nodes.count(); i++)
                {
                    QDomElement child=nodes.at(i).toElement();
                    if(child.nodeName() == "Type")
                    {
                        QDomNodeList subnodes=child.childNodes();
                        for(int j = 0; j < subnodes.count(); j++)
                        {
                            if(subnodes.at(j).nodeName() == "Field")
                            {
                                m_strTypeField = subnodes.at(j).toElement().text();
                            }
                            if(subnodes.at(j).nodeName() == "Value")
                            {
                                m_strTypeValue = subnodes.at(j).toElement().text();
                            }
                        }
                    }
                    if(child.nodeName() == "Data")
                    {
                        QDomNodeList subnodes=child.childNodes();
                        for(int j = 0; j < subnodes.size(); j++)
                        {
                            if(subnodes.at(j).nodeName() == "Field")
                            {
                                m_strDataField = subnodes.at(j).toElement().text();
                            }
                            if(subnodes.at(j).nodeName() == "Value")
                            {
                                m_strDataValue = subnodes.at(j).toElement().text();
                            }
                        }
                    }
                }
            }


        }
        //下一个兄弟节点,nextSiblingElement()是下一个兄弟元素，都差不多
        node=node.nextSibling();
    }
    return PrintFieldList;
}

void ConfigManager::WriteConfig(QList<QString> fields)
{
    //打开或创建文件
    //相对路径、绝对路径、资源路径都可以
    QFile file("config.xml");

    //可以用QIODevice，Truncate表示清空原来的内容
    if(!file.open(QFile::WriteOnly|QFile::Truncate))
    {
        return;
    }

    QDomDocument doc;

    //写入xml头部
    QDomProcessingInstruction instruction; //添加处理命令
    instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    //添加根节点
    QDomElement root=doc.createElement("PrintFieldList");
    doc.appendChild(root);

    for(int i = 0;  i < fields.size(); i++)
    {
        //添加子节点
        QDomElement book=doc.createElement("Field");
        //设置括号标签中间的值
        QDomText text;
        text=doc.createTextNode(fields[i]);
        book.appendChild(text);
        root.appendChild(book);
    }
    //输出到文件
    QTextStream out_stream(&file);

    //缩进4格
    doc.save(out_stream,4);
    file.close();

}

QString ConfigManager::GetPrintTitle()
{
    return m_strTitle;
}

QString ConfigManager::GetPrintSubTitle()
{
    return m_strSubTitle;
}

QString ConfigManager::GetTypeField()
{
    return m_strTypeField;
}

QString ConfigManager::GetTypeValue()
{
    return m_strTypeValue;
}

QString ConfigManager::GetDataField()
{
    return m_strDataField;
}

QString ConfigManager::GetDataValue()
{
    return m_strDataValue;
}
