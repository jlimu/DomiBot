# -*- coding: utf-8 -*-
"""
Created on Mon Apr 13 18:16:39 2020

@author: Sergi Pérez
"""


class Ficha():
    
    def __init__(self, tupla):
        
        self.__ficha = tupla
        
        if self.__ficha[0] != self.__ficha[1]:
            
            self.__up = None
            self.__down = None
        
    
    def __setRight__(self, ficha):
        self.__right = ficha
        
    def __setLeft__(self, ficha):
        self.__left = ficha
        
    def __setUp__(self, ficha):
        self.__up = ficha


    
    class Palabra():
    def __init__(self, ficha):
        self.__palabra = " "
        self.__origen = Punto()
        self.__longitud = 0
        self.__orientacion = ' '