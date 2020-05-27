# -*- coding: utf-8 -*-
"""
Created on Sat Apr 25 18:05:47 2020

@author: Sergi Pérez
"""
import numpy as np

class TableroV1:
    
    def __init__(self):
        
        self.__tablero = []
    
    
    def añadirFicha(self, ficha, pos):
        
        if len(self.__tablero) == 0:
            
            self.__tablero.append(ficha)
            return True
        
            
            
        elif pos == 1:
           if self.__tablero[0][0] == ficha[1]:
               self.__tablero.insert(0, ficha)
               return True
               
           elif self.__tablero[0][0] == ficha[0]:
               ficha = ficha[::-1]
               self.__tablero.insert(0, ficha)
               return True
           
           else: return False
               
        elif pos == 2:
            
            if self.__tablero[len(self.__tablero)-1][1] == ficha[0]:
               self.__tablero.append(ficha)
               return True
               
            elif self.__tablero[len(self.__tablero)-1][1] == ficha[1]:
               ficha = ficha[::-1]
               self.__tablero.append(ficha)
               return True
               
            else: return False
           
    
    def getExtremos(self):
        
        extremos = []
        
        extremos.append([self.__tablero[0][0], [0]])
        extremos.append([self.__tablero[len(self.__tablero)-1][1], [len(self.__tablero)]])
        
        return extremos
        
    
    def getFichas(self):
        
        return self.__tablero
    
    def printTablero(self):
        
        print(self.__tablero)
        
        