# -*- coding: utf-8 -*-
"""
Created on Wed Dec 12 22:14:41 2018

@author: Vedhus
"""

from windows import Window
from PyQt5 import QtWidgets
import sys


                
if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    window = Window()
    window.setGeometry(500, 300, 800, 600)
    window.show()
    sys.exit(app.exec_())
    
    