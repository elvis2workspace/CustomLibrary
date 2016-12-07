#coding:utf-8
#file:pyoptimizel.py
import tkinter
import tkinter.messagebox
class window:
    def __init__(self):
        self.root=tkinter.Tk()
        #创建菜单
        menu=tkinter.Menu(self.root)
       
   
    
        #创建”系统”子菜单
        submenu=tkinter.Menu(menu,tearoff=0)
        submenu.add_command(label="关于...",command=self.MenuAbout)
    #"关于"菜单
    def MenuAbout(self):
        tkinter.messagebox.showinfo("PyOptimize","这是使用Python编写的windows优化程序。\n欢迎使用并提出意见！")
   
         
        submenu.add_separator()
        submenu.add_command(label="退出",command=self.MenuExit)
     #"退出"菜单
    def MenuExit(self):
        self.root.quit();
    
        #创建"清理"子菜单
        submenu=tkinter.Menu(menu,tearoff=0)
        submenu.add_command(label="扫描垃圾文件",command=self.MenuScanRubbish)
      #"扫描垃圾文件"菜单
    def MenuScanRubbish(self):
        result=tkinter.messagebox.askquestion("pyOptimize","扫描埒文件将需要较长时间，是否继续？")
        if result=='no':
            return
        tkinter.messagebox.showwinfo("pyoptimize","马上开始删除垃圾文件！")
   
        submenu.add_command(label="删除垃圾文件",connand=self.MenuDelRubbish)
     #"删除垃圾文件"菜单
    def MenuDelRubbish(self):
        result=tkinter.messagebox.askquestion("pyoptimize","删除垃圾文件将需要较长时间，是否继续？")
        if result=='no':
            return
        tkinter.messagebox.showinfo("pyoptimize","马上开始删除垃圾文件！")
        
        menu.add_cascade(label="清理",menu=submenu)
             
        #创建"查找"子菜单
        submenu=tkinter.Menu(menu,tearoff=0)
        
        submenu.add_command(label="搜索大文件",connand=self.MenuScanBigFile)
    #"搜索大文件"菜单
    def MenuScanBigFile(self):
        result=tkinter.messagebox.askquestion("pyoptimize","扫描大文件将需要较长的时间，是否继续？")
        if result=='no':
            return
        tkinter.messagebox.showinfo("pyoptimize","马上开始扫描大文件！")
        

        submenu.add_separator()
        submenu.add_command(label="按名称搜索文件",command=self.MenuSearchFile)
        menu.add_cascade(label="搜索",menu=submenu)
        self.root.config(menu=menu)
     
        #创建标签，用于显示状态信息
        self.progress=tkinter.Label(self.root,anchor=tkinter.W,text='状态',bitmap='hourglass',compound='left')
        self.progress.place(x=10,y=370,width=460,height=16)
        #创建文本框，显示文件列表
        self.flist=tkinter.Text(self.root)
        self.flist.place(x=10,y=10,width=460,height=350)
         #为文本框添加垂直滚动条
        self.vscroll=tkinter.Scrollbar(self.flist)
        self.vscroll.pack(side='right',fill='y')
        self.flist['yscrollcommand']=self.vscroll.set
        self.vscroll['command']=self.flist.yview
    def mainloop(self):
        self.root.title("Pyoptimeize")
        self.root.minsize(500,400)
        self.root.maxsize(500,400)
        self.root.mainloop()
if __name__=="__main__":
    window=window()
    window.mainloop()

