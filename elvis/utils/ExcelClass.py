#coding:utf-8
import win32com.client
import os
OK_COLOR = 0XFFFFE0
NG_COLOR = 0xff
NT_COLOR = 0XC0C0C0

class ExcelClass:
    def __init__(self):
        self.book = None
        self.sfile =None
    def setup(self,sfile):
        self.xlApp = win32com.client.Dispatch("Excel.Application")
        try:
            self.book = self.xlApp.Workbooks.Open(sfile)
        except:
            print ("鎵撳紑澶辫触")
            exit()
        self.sfile = sfile
        
    def close(self):
        self.book.Save()
        self.book.Close()
        del self.xlApp
    #��ȡ���
    def read_data(self, isheet , iRow ,iCol):
        '''读取excel数据，三个必传参数sheet铭，行，列'''
        try:
            sht = self.book.Worksheets(isheet)
            sValue = str(sht.Cells(iRow,iCol).value)
        except:
            self.close()
            print('读取数据失败')
            exit()
        if sValue[-2:]=='.0':
            sValue = sValue[0:-2]
        return sValue
        
    def write_data(self, isheet , iRow ,iCol ,sData , color = OK_COLOR):
        '''写数据，sheet名，行，列，数据'''
        try:
            sht = self.book.Worksheets(isheet)
            sht.Cells(iRow,iCol).value = sData #.decode("utf-8")
            sht.Cells(iRow,iCol).interior.Color = color
            self.book.Save()
        except:
            self.close()
            print('写失败')
            exit()
            
    def get_nrows(self,isheet):
        '''获取sheet行数'''
        try:
            sht = self.book.Worksheets(isheet)
            return sht.UsedRange.Rows.Count
        except:
            self.close()
            print('获取行失败')
            exit()
            
    def get_ncols(self,isheet):
        '''获取sheet列数'''
        try:
            sht = self.book.Worksheets(isheet)
            return sht.UsedRange.Columns.Count    
        except:
            self.close()
            print('列失败')
            exit()
    #��ȡ�������
    def get_ncase(self,isheet):
        '''获取sheet用例数'''
        try:
            sht = self.book.Worksheets(isheet)
            return self.get_nrows(isheet)- self.casebegin+1    
        except:
            self.close()
            print('获取用例数失败')
            exit()
    
    def del_testrecord(self,suiteid):
        '''清除数据'''
        try:
            nrows = self.get_nrows(suiteid)+1
            
            ncols = self.get_ncols(suiteid)+1
            resbegincol = self.argbegin +self.argcount
            sht = self.book.Worksheets(suiteid)
            for row in range(self.casebegin,nrows):
                for col in range(resbegincol,ncols):
                    str=self.read_data(suiteid, row, col) 
                    startpos = str.find('[')
                    if startpos>0:
                        str = str[0:startpos].strip()
                        self.write_data(suiteid, row ,col ,str , OK_COLOR)
                    else:
                        sht.Cells(row,col).Interior.Color = OK_COLOR
                self.write_data(suiteid,row,self.xmlCol,' ',OK_COLOR)
                self.write_data(suiteid ,row , self.resultCol,'NT',NT_COLOR)
        except:
            self.close()
            print("清除失败")
            exit()

