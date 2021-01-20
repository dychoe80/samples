#!/bin/python3
# -*- coding: utf-8 -*-

# References:
# https://gist.github.com/moshekaplan/c425f861de7bbf28ef06
# https://www.iditect.com/how-to/53371117.html
# https://beenje.github.io/blog/posts/logging-to-a-tkinter-scrolledtext-widget/
# https://stackoverflow.com/questions/13318742/python-logging-to-tkinter-text-widget


import logging
import queue
import threading
import time
import tkinter


class TextHandler(logging.Handler):
    def __init__(self, text_widget):
        logging.Handler.__init__(self)
        self.text_widget = text_widget
        self.queue = queue.Queue()
        self.text_widget.after(100, self.poll_queue)  # Start polling message from the queue

    def close(self):
        self.queue.join()

    def emit(self, record):
        self.queue.put(record)

    def poll_queue(self):
        # Check every 100 ms if there is a new message in the queue to display
        while not self.queue.empty():
            msg = self.queue.get(block=False)
            self.text_widget.configure(state='normal')
            self.text_widget.insert(tkinter.END, self.format(msg) + '\n')
            self.text_widget.configure(state='disabled')
            self.text_widget.yview(tkinter.END)  # Autoscroll to the bottom
            self.queue.task_done()
        self.text_widget.after(100, self.poll_queue)


class LogWidget(tkinter.Frame):
    def __init__(self, logfile=None, *args, **kwargs):
        super().__init__(*args, **kwargs)

        tkinter.Button(self, text='Clear', command=self.clear).grid(row=0, column=0)

        self.text = tkinter.Text(self, wrap='none', borderwidth=0)
        text_vsb = tkinter.Scrollbar(self, orient='vertical', command=self.text.yview)
        text_hsb = tkinter.Scrollbar(self, orient='horizontal', command=self.text.xview)
        self.text.configure(yscrollcommand=text_vsb.set, xscrollcommand=text_hsb.set, font='TkFixedFont')

        self.text.grid(row=1, column=0, sticky='nsew')
        text_vsb.grid(row=1, column=1, sticky='ns')
        text_hsb.grid(row=2, column=0, sticky='ew')

        self.grid_rowconfigure(1, weight=1)
        self.grid_columnconfigure(0, weight=1)

        self.pack(side='top', fill='both', expand=True)

        # logging.basicConfig(filename=logfile, level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

        self.text_handler = TextHandler(self.text)
        formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
        self.text_handler.setFormatter(formatter)

        logger = logging.getLogger()
        logger.addHandler(self.text_handler)
        logger.setLevel(logging.INFO)

    def destroy(self):
        logging.getLogger().removeHandler(self.text_handler)

    def clear(self):
        self.text.configure(state='normal')
        self.text.delete('1.0', tkinter.END)
        self.text.configure(state='disabled')


def main():
    root = tkinter.Tk()
    LogWidget(master=root)

    def worker():
        while True:
            time.sleep(1)
            msg = f'Current time: {time.asctime()}'
            logging.info(msg)

    t1 = threading.Thread(target=worker, args=[])
    t1.start()

    root.mainloop()
    t1.join()


if __name__ == "__main__":
    main()
