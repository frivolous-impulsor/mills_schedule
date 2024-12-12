import ctypes

def Mbox(title, text, style):
    return ctypes.windll.user32.MessageBoxW(0, text, title, style)

Mbox("my title", "my text\nnext line text\n\tthird with tab", 0)