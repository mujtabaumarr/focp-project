from PIL import Image, ImageDraw, ImageFont

# Create a series of characters with gradient effect
def create_character_images(characters, output_folder):
    font = ImageFont.truetype("arial.ttf", 100)  # Use your preferred font
    for i, char in enumerate(characters):
        # Define gradient color
        gray_value = int(255 * (i / (len(characters) - 1)))  # Black to white
        color = (gray_value, gray_value, gray_value)

        # Create image
        img = Image.new("RGBA", (200, 200), (255, 255, 255, 0))  # Transparent
        draw = ImageDraw.Draw(img)
        text_width, text_height = draw.textsize(char, font=font)
        position = ((200 - text_width) // 2, (200 - text_height) // 2)

        # Draw character
        draw.text(position, char, fill=color, font=font)

        # Save as PNG
        img.save(f"{output_folder}/{char}.png")

# Usage
characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
create_character_images(characters,"./output")