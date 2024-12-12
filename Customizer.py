import serial
from tkinter import Tk, Label, Button, Scale, HORIZONTAL
from pynput.mouse import Listener

# 아두이노 연결 설정
arduino = serial.Serial('COM5', 115200)

# 기본값
current_pattern = "S"  # 기본 패턴
current_strength = 255  # 기본 강도 (최대값)
stop_active = False  # STOP 상태 플래그

# 진동 패턴 정의
patterns = {
    "Single": ("S", "lightblue"),
    "Double": ("D", "lightgreen"),
    "Hold": ("H", "yellow"),
    "Burst": ("B", "orange"),
    "Wave": ("W", "pink"),
    "Pulse": ("P", "purple"),
    "Random": ("R", "cyan"),
    "Heartbeat": ("B1", "gold"),
    "Stutter": ("T", "lime")
}

def send_vibration_signal():
    """
    현재 설정된 패턴과 강도를 아두이노로 즉시 전송
    """
    global stop_active
    if not stop_active:
        command = f"{current_pattern}{current_strength}\n"
        arduino.write(command.encode('utf-8'))
        arduino.flush()
        print(f"Sent: {command.strip()}")


def on_click(x, y, button, pressed):
    """
    마우스 왼쪽 클릭 이벤트 처리
    """
    if pressed:
        print(f"Mouse clicked with pattern: {current_pattern}, strength: {current_strength}")
        send_vibration_signal()


def update_pattern(pattern):
    """
    진동 패턴 업데이트
    """
    global current_pattern, stop_active
    stop_active = False  # STOP 상태 해제
    current_pattern = patterns[pattern][0]
    print(f"Pattern updated to: {pattern}")


def update_strength(value):
    """
    진동 강도 업데이트
    """
    global current_strength
    current_strength = int(value)
    print(f"Strength updated to: {current_strength}")


def stop_vibration():
    """
    STOP 버튼 기능: 진동을 완전히 멈춤
    """
    global stop_active
    stop_active = True
    arduino.write(b'0\n')  # 진동 멈춤 명령 전송
    arduino.flush()
    print("Vibration stopped.")

# Tkinter UI 생성
root = Tk()
root.title("Mouse Vibration Customizer")

# 패턴 선택 버튼
pattern_label = Label(root, text="Select Vibration Pattern:")
pattern_label.pack()

for pattern, (cmd, color) in patterns.items():
    Button(root, text=pattern, bg=color, fg="black", command=lambda p=pattern: update_pattern(p)).pack(fill="x", pady=2)

# 강도 선택 슬라이더
strength_label = Label(root, text="Select Vibration Strength:")
strength_label.pack()

strength_slider = Scale(root, from_=0, to=255, orient=HORIZONTAL, command=update_strength)
strength_slider.set(current_strength)
strength_slider.pack()

# STOP 버튼
stop_button = Button(root, text="STOP", bg="red", fg="white", command=stop_vibration)
stop_button.pack(fill="x", pady=10)

# 상태 표시
status_label = Label(root, text="Mouse Vibration is Active")
status_label.pack()

# 마우스 리스너 시작
with Listener(on_click=on_click) as mouse_listener:
    root.mainloop()
    mouse_listener.stop()
