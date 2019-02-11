import tensorflow.examples.tutorials.mnist.input_data as mnist_input_data
import tensorflow as tf
import tensorflow.contrib.slim as slim
import numpy as np
import os
import tkinter
from PIL import Image,ImageDraw

from YetLeNet import LeNet

MODEL_FILE_NAME = 'model/model.save'

class Checker:
    def __init__(self):
        self.lenet = LeNet()
        self.sess = tf.Session()
        self.saver = tf.train.Saver()
    def predict(self, image):
        img = image.convert('L')
        img = img.resize([28, 28], Image.ANTIALIAS)
        image_input = np.array(img, dtype='float32') / 255
        image_input = np.reshape(image_input, [-1, 784])
        self.saver.restore(self.sess, MODEL_FILE_NAME)
        return self.sess.run(self.lenet.prediction, feed_dict={self.lenet.raw_input_image: image_input})        

class MyCanvas:
    def __init__(self, parent):
        self.parent=parent
        self.canvas=tkinter.Canvas(parent, width=256, height=256, bg='black')
        self.canvas.pack()
        self.image1 = Image.new('RGB', (256, 256), 'black')
        self.draw = ImageDraw.Draw(self.image1)
        self.canvas.bind('<B1-Motion>', self.Draw)


    def Draw(self,event):
        self.canvas.create_oval(event.x,event.y, event.x,event.y, outline='white',width = 20)
        self.draw.ellipse((event.x-10, event.y-10, event.x+10, event.y+10), fill=(255,255,255))


def main():
    root = tkinter.Tk()
    root.geometry('300x400')
    frame = tkinter.Frame(root, width=256, height=256)
    frame.pack_propagate(0)
    frame.pack(side='top')
    canvas1 = MyCanvas(frame)
    checker = Checker()

    def recognition_on_click():
        img = canvas1.image1
        # N * 1 array
        # where N == 1
        # 1 \in {0, ..., 9}
        result = checker.predict(img)
        # show the result
        result = int(result[[0]])

        canvas1.canvas.delete('all')
        canvas1.image1 = Image.new('RGB', (256, 256), 'black')
        canvas1.draw = ImageDraw.Draw(canvas1.image1)
        label2['text'] = str(result)

    botton_recognition = tkinter.Button(root,
                                      text='Recognition',
                                      width=7,
                                      height=1,
                                      command=recognition_on_click
                                      )
    botton_recognition.pack()
    label1 = tkinter.Label(root, justify='center', text='Recognition result is')
    label1.pack()
    label2 = tkinter.Label(root, justify='center')
    label2['font'] = ('Arial, 48')
    label2.pack()
    root.mainloop()

if __name__ == '__main__':
    main()
