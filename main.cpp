#include <SFML/Graphics.hpp>
#include <array>
#include <random>
#include <string>

using namespace sf;

// ���� ������ ��� �������� ���������� �� ���������� ��������� ���� � ���� ��� ��������
std::array <std::array <int, 10>, 10> field_in{ 0 };
std::array <std::array <int, 10>, 10> field_out{ 0 };

void fEvent(RenderWindow& window, bool game_over = false);
int fIsGameOver(int seconds = 1);
void fPrintText(std::string text, Font& font, RenderWindow& window, float x_correction = 0, float y_correction = 0);
std::string fTime(int seconds);
int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(384, 384), "Minesweeper", Style::Default, settings);

	// ������� � ��������� ��������
	Texture texture_cube;
	texture_cube.loadFromFile("C:\\Users\\Finchi\\source\\repos\\SFML_Mineswepper\\Minesweeper.jpg");
	Texture texture_reset;
	texture_reset.loadFromFile("C:\\Users\\Finchi\\source\\repos\\SFML_Mineswepper\\Reset.png");

	// ������� � ��������� �����
	Font font;
	font.loadFromFile("C:\\Users\\Finchi\\source\\repos\\SFML_Mineswepper\\CyrilicOld.TTF");

	// ������ ���� �������� ��� ����� �� �� ����
	std::array <Sprite, 12> sprites;
	for (int s = 0; s < sprites.size(); s++)
	{
		sprites.at(s).setTexture(texture_cube);
		sprites.at(s).setTextureRect(IntRect(s * 32, 0, 32, 32));
	}

	// ��������� �������� "��������" ������� ���������� ����������
	for (int x = 0; x < field_out.size(); x++)
		for (int y = 0; y < field_out.at(x).size(); y++)
			field_out.at(x).at(y) = 10;

	// ��������� �������� "�����������" ������� ���������� ����������
	// ��� ����� ��� ������ ����� ���� ����� ������� ������ Reset, ����� � ������� ���������� ������ ��������
	for (int x = 0; x < field_in.size(); x++)
		for (int y = 0; y < field_in.at(x).size(); y++)
			field_in.at(x).at(y) = 0;

	// ������� ����� �� "����������" �������
	for (int x = 0; x < field_in.size(); x++)
		for (int y = 0; y < field_in.at(x).size(); y++)
			if (std::mt19937(std::random_device{}())() % 5 == 0)
				field_in.at(x).at(y) = 9;

	// ��������� ���������� ���� � �������� ������ "�����������" ����
	for (int x = 0; x < field_in.size(); x++)
		for (int y = 0; y < field_in.at(x).size(); y++)
		{
			// ���� � ������ �����, �� ����� �������� � ��������� ��������
			if (field_in.at(x).at(y) == 9)
				continue;
			// ��������� ��� ������ ������ �������, ���� ������� �����, �� ��������� � ������� ������ +1
			for (int i = x - 1; i <= x + 1; i++)
				for (int j = y - 1; j <= y + 1; j++)
					if (i >= 0 && i < field_in.size() && j >= 0 && j < field_in.at(x).size() && field_in.at(i).at(j) == 9)
						field_in.at(x).at(y)++;
		}

	// ����
	Clock clock;
	clock.restart();
	int temp_seconds = 0;

	// ������ �����
	Sprite sprite_reset(texture_reset);
	sprite_reset.setPosition(5.55f * 32, 11.1f * 32);
	sprite_reset.scale(0.8f, 0.8f);

	while (window.isOpen())
	{
		window.clear();
		// ��������� �������� ���� ("��������" �������)
		for (int x = 0; x < field_out.size(); x++)
			for (int y = 0; y < field_out.at(x).size(); y++)
			{
				sprites.at(field_out.at(x).at(y)).setPosition(32 + x * 32.f, 32 + y * 32.f);
				window.draw(sprites.at(field_out.at(x).at(y)));
			}

		// �������� �� �������� �� ����
		int game_over = fIsGameOver(600 - temp_seconds);
		if (game_over == -1)
		{
			field_out = field_in;
			fPrintText("Game over", font, window, 4.5f * 32);
			window.draw(sprite_reset);
		}
		else if (game_over == 1)
		{
			fPrintText("You win!", font, window, 4.8f * 32);
			window.draw(sprite_reset);
		}

		// ����� ��������� ������� �� �����
		// ���� ������ ���� �������
		if (temp_seconds != clock.getElapsedTime().asSeconds() && game_over == 0)
		{
			temp_seconds = static_cast<int>(clock.getElapsedTime().asSeconds());
			fPrintText("Timer: " + fTime(600 - temp_seconds), font, window, 4.3f * 32, 11.f * 32);
		}

		// ���������� �������
		fEvent(window, game_over == -1 || game_over == 1);

		// ��������� ����
		window.display();
	}
	return 0;
}

// ���������� �������
void fEvent(RenderWindow& window, bool game_over)
{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			window.close();
		// ������� �������� ����
		if (event.type == Event::MouseButtonPressed)
		{
			Vector2i pos = Mouse::getPosition(window);
			int x = pos.x / 32 - 1;
			int y = pos.y / 32 - 1;
			// ���� ���������� �������� �� ������� ����
			if (x >= 0 && x < field_out.size() && y >= 0 && y < field_out.at(0).size())
			{
				// �����
				if (event.key.code == Mouse::Left)
					field_out.at(x).at(y) = field_in.at(x).at(y);
				// ������ - ������ � ������� ����
				if (event.key.code == Mouse::Right)
				{
					if (field_out.at(x).at(y) == 10)
						field_out.at(x).at(y) = 11;
					else if (field_out.at(x).at(y) == 11)
						field_out.at(x).at(y) = 10;
				}
			}
			// ���� ���������� �� �������� �� ������� ����, � ���� ���������, �� ��������� ������ �� ������ �����
			else
				if (game_over && event.key.code == Mouse::Left)
					if (x >= 4 && x <= 5 && y == field_out.at(0).size())
					{
						window.close();
						main();
					}
		}
	}
}

// ��������� -1 ���� ���������, 0 ���� ���� ��� �� ��������� � 1 ���� ��������
int fIsGameOver(int seconds)
{
	// ���� ����� �����������, �� ���� ��������� ����������
	if (seconds <= 0)
		return -1;
	// ���� ������� ���� ���� �����, �� ���� ��������� ����������
	for (int x = 0; x < field_out.size(); x++)
		for (int y = 0; y < field_out.at(x).size(); y++)
			if (field_out.at(x).at(y) == 9)
				return -1;
	// ���� ������� ��� ����, � �� ���� ������ ������������ ������, �� ���� ���������� �������
	for (int x = 0; x < field_out.size(); x++)
		for (int y = 0; y < field_out.at(x).size(); y++)
		{
			// ���� ������� ���� ���� ���������� �������, �� ���� ������������
			if (field_out.at(x).at(y) == 10)
				return 0;
			// ���� ���� �� ����� ����� �� ����� ����, �� ���� ������������
			if (field_in.at(x).at(y) == 9)
				if (field_out.at(x).at(y) != 11)
					return 0;
			// ���� ���� ����� �� �� �����, �� ���� ������������
			if (field_in.at(x).at(y) != 9 && field_out.at(x).at(y) == 11)
				return 0;
		}
	return 1;
}

// ������� ����� �� �����
void fPrintText(std::string text, Font& font, RenderWindow& window, float x_correction, float y_correction)
{
	Text stext(text, font, 20); //����� �� ������� 20
	stext.setPosition(x_correction, y_correction); //������������� ������ ������
	stext.setFillColor(Color::Red); //���� �������
	window.draw(stext);
}

// ����� � �������� ��������� � ���� � ������, ���������� � ������
std::string fTime(int seconds)
{
	std::string result;
	int iminutes = seconds / 60;
	int iseconds = seconds % 60;
	std::string sminutes = std::to_string(iminutes);
	std::string sseconds = std::to_string(iseconds);
	if (iminutes < 10)
		sminutes.insert(sminutes.begin(), '0');
	if (iseconds < 10)
		sseconds.insert(sseconds.begin(), '0');
	result += sminutes + ":" + sseconds;
	return result;
}
