import cv2 as cv
import numpy as np

def processo_immagine_incroci(img):
    copia = np.copy(img)    #copia di ogni frame

    alt = img.shape[0]
    base = img.shape[1]
    
    SCARTO = 20         #SCARTO che indica la larghezza/2 della linea
    SCARTO_ZONA = 50    #SCARTO per la zona di interesse dei quadrati verdi

    #coordinate per la maschera per le zone di interesse,viene definito prima il quadrato sinistro e poi quello destro
    cord1 = np.array([[
        (base/2-(SCARTO+SCARTO_ZONA),alt/2+SCARTO),
        (base/2-(SCARTO+SCARTO_ZONA),alt/2+SCARTO+SCARTO_ZONA),
        (base/2-SCARTO,alt/2+SCARTO+SCARTO_ZONA),
        (base/2-SCARTO,alt/2+SCARTO)
    ],
    [
        (base/2+(SCARTO+SCARTO_ZONA),alt/2+SCARTO),
        (base/2+(SCARTO+SCARTO_ZONA),alt/2+SCARTO+SCARTO_ZONA),
        (base/2+SCARTO,alt/2+SCARTO+SCARTO_ZONA),
        (base/2+SCARTO,alt/2+SCARTO)
    ]],dtype=np.int32)

    #conversione dell'immagine da BGR a HSV per l'individuazione dei colori
    hsv = cv.cvtColor(copia, cv.COLOR_BGR2HSV)

    #individuazione del verde e creazione della maschera per isolarlo
    copia_v = cv.inRange(hsv,(36,0,0),(86,255,255)) 
    imask = copia_v>0
    green = np.zeros_like(copia, np.uint8)
    green[imask] = copia[imask]

    green_int = zona_di_interesse_col(green,cord1)

    #coordinate del centro dei quadrati
    centro_qd = [[int(base/2+(SCARTO)+SCARTO_ZONA/2)],[int(alt/2+(SCARTO)+SCARTO_ZONA/2)]]
    centro_qs = [[int(base/2-(SCARTO)-SCARTO_ZONA/2)],[int(alt/2+(SCARTO)+SCARTO_ZONA/2)]]
    #//debug
    print(green_int[centro_qd[1][0]][centro_qd[0][0]])

    cv.circle(green_int,(centro_qd[0][0],centro_qd[1][0]),2,255)
    cv.circle(green_int,(centro_qs[0][0],centro_qs[1][0]),2,255)
    #debug//

    #controllo che i centri dei quadrati non contengano verde
    #se contengono verde eseguo le operazioni corrispondenti
    if( np.any(green_int[centro_qd[1][0]][centro_qd[0][0]]) and  np.any(green_int[centro_qs[1][0]][centro_qs[0][0]])):
        print("GIRA 180")
    elif( np.any(green_int[centro_qd[1][0]][centro_qd[0][0]])):
        print("DESTRA")
    elif( np.any(green_int[centro_qs[1][0]][centro_qs[0][0]])):
        print("SINISTRA")
    else:
        print("DRITTO")



    return green_int

#calcolo del punto medio dato un lista conente due coordinate x
def punto_medio(x):
    if(len(x) > 1):
        return (x[0]+x[1])/2

#trova le coordinate di intersezzione con i punti di interesse nell'immagine a cui è stato applicato il canny
def trova_coord(img,punti_int):
    SCARTO = 20 #range attorno ai punti di interesse
    x = []  #coordinate di ritorno
    i = img[punti_int[0][1]]    #prendo la riga dell'immagine che mi interessa

    #itero i punti che interessano per trovare l'intersezione
    for j in range(int(punti_int[0][0]-SCARTO),int(punti_int[0][0]+SCARTO)):
        if(i[j] != 0):
            x.append(j)
    
    for j in range(int(punti_int[1][0]-SCARTO),int(punti_int[1][0]+SCARTO)):
        if(i[j] != 0):
            x.append(j)

    #//debug
    cv.line(img,(int(punti_int[0][0]-SCARTO),int(punti_int[0][1])),(int(punti_int[0][0]+SCARTO),int(punti_int[0][1])),100,1)
    cv.line(img,(int(punti_int[1][0]-SCARTO),int(punti_int[1][1])),(int(punti_int[1][0]+SCARTO),int(punti_int[1][1])),100,1)
    if(len(x) >1):
        cv.circle(img,(x[0],int(punti_int[0][1])),2,255,2)
        cv.circle(img,(x[1],int(punti_int[1][1])),2,255,2)
    #debug//

    return x

#definisce la zone di interesse data un'immagine con un singolo canale e i punti dei vertici della zona
def zona_di_interesse(img,cord):
    img_v = np.zeros_like(img)  #creado della matrice(immagine) con dentro solo zeri
    cv.fillPoly(img_v,cord,255) #creo un poligono che ha costitusce la mia zona di interesse, questo è formato solo da 255(8 bit a 1 in binario)
    img_a = np.bitwise_and(img_v,img)   #faccio & per tutti i bit così da ottenere la mia zona di interesse
    return img_a

#defisce la zona di interesse come per zona_di_interesse() solo che lo fa per immagini con più di un canale, così da preservare il colore
def zona_di_interesse_col(img,cord):
    img_v = np.zeros_like(img)
    col = (255,)*img.shape[2]   #calcolo del colore
    cv.fillPoly(img_v,cord,col)
    img_a = np.bitwise_and(img_v,img)
    return img_a


def processo_immagine_linee(img):
    alt = img.shape[0]  #altezza immagine
    base = img.shape[1] #base immagine
    SCARTO = 100    #scarto per definire i vertici della zona di interesse
    SCARTO_PNT = 10 #scarto per tenere la linea

    #vertici per la zona di interesse
    cord = np.array([[
        (base/2-SCARTO,alt/2),
        (base/2-SCARTO,alt/2+SCARTO),
        (base/2+SCARTO,alt/2+SCARTO),
        (base/2+SCARTO,alt/2)
    ]],dtype=np.int32)

    copia = np.copy(img)    #copia del frame passato come parametro
    copia_g = cv.cvtColor(copia,cv.COLOR_BGR2GRAY)  #conversione a bianco e nero per avere un singolo canale
    copia_c = cv.Canny(copia_g,50,150)  #trovo gli angoli nel frame

    copia_int = zona_di_interesse(copia_c,cord)

    base_zona_int = (base/2+SCARTO) - (base/2-SCARTO)   #calcolo per la base della zona di interesse

    #punti di interesse per la ricereca della linea, equacalgono a 1/3 e 2/3 della base della zona di interesse
    punti_di_int = [
        [base/2-SCARTO+(base_zona_int/3),int(alt/2+SCARTO/2)],
        [base/2-SCARTO+((base_zona_int/3)*2),int(alt/2+SCARTO/2)]
    ]

    x = trova_coord(copia_int,punti_di_int) #coordinate delle intersezioni 
    pnt_med = punto_medio(x)    #calcolo del punto medio delle coordinate x
    print(pnt_med)

    #//debug
    cv.line(copia_int,(int(base/2),int(punti_di_int[0][1]+10)),(int(base/2),int(punti_di_int[0][1])-10),100,1)
    #debug//
    if(pnt_med != None):    #se il punto medio non è stato calcolato vuole dire che ci si trova in prossimità di una curva di 90°
    #//debug
        cv.circle(copia_int,(int(pnt_med),int(punti_di_int[0][1])),2,255,2)
    #debug//

        # se il punto medio non è centrato viene comunicato l'operazione da eseguire
        if(pnt_med < base/2-SCARTO_PNT):    
            print("SINISTRA")
        elif(pnt_med > base/2+SCARTO_PNT):
            print("DESTRA") 
        else:
            print("DRITTO")
    elif(len(x) > 0):   #curva a 90°
        if(x[0] > base/2):
            print("SINISTRA")
        else:
            print("DESTRA")
    else:
        print("DRITTO")


    return x,copia_int

if(__name__ == "__main__") :
    
    #nel main c'è solo la cattura della telecamera

    cam = cv.VideoCapture(0)

    while(True):
        ret,frame = cam.read()  #ogni frame viene processato
        if(not ret):
            print("ERRORE")
            break

        x,img = processo_immagine_linee(frame)      #operazioni per seguire la linea

        if(len(x)<= 0): #se non ci sono interesezioni con le linee ci si trova in prossimità di un incrocio
            processo_immagine_incroci(frame)       #operazioni per scegliere la direzione

        cv.imshow("c",img)
        k = cv.waitKey(1)

        if (k%257 == 27):
            break
