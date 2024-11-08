#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include<iostream>

const double larguraTela = 800;
const double alturaTela = 600;
const double larguraRaquete = 20;
const double alturaRaquete = 100;
const double raioBola = 10;
const double velBola = 0.2;

class Bola {
    sf::CircleShape forma;
    sf::Vector2f velocidade;
public:

    Bola(double raio) : forma(raio), velocidade(-velBola, -velBola) {
        forma.setFillColor(sf::Color::White);
        forma.setPosition(larguraTela / 2, alturaTela / 2);
    }

    void mover() {
        forma.move(velocidade);

        if (forma.getPosition().y < 0 || 
            forma.getPosition().y + raioBola * 2 > alturaTela)
            velocidade.y = -velocidade.y;
    }

    void resetar() {
        forma.setPosition(larguraTela / 2, alturaTela / 2);
        velocidade.x = -velocidade.x;
        velocidade.y = (rand() % 2 == 0) ? -velBola : velBola;
    }

    sf::CircleShape shape() const { return forma; }

    double getX() const { return forma.getPosition().x; }
    double getY() const { return forma.getPosition().y; }

    void inverteX() { velocidade.x = -velocidade.x; }
    void inverteY() { velocidade.y = -velocidade.y; }
};

class Raquete {
    sf::RectangleShape forma;
    double velocidade;
public:
    Raquete(double x, double y) : forma(sf::Vector2f(larguraRaquete, alturaRaquete)), velocidade(0.5f) {
        forma.setFillColor(sf::Color::White);
        forma.setPosition(x, y);
    }

    void mover(double deltaY) {
        if (forma.getPosition().y + deltaY >= 0 && forma.getPosition().y + deltaY + alturaRaquete <= alturaTela)
            forma.move(0, deltaY);
    }

    void moverAutomatica(double bolaY) {
        if(bolaY < forma.getPosition().y)
            mover(-velocidade);
        else if(bolaY > forma.getPosition().y + alturaRaquete)
            mover(velocidade);
    }

    sf::RectangleShape shape() const { return forma; }
    void moveUp() { mover(-velocidade); }
    void moveDown() { mover(velocidade); }

    bool colide(const Bola& b) const {
        return b.shape().getGlobalBounds().intersects(forma.getGlobalBounds());
    }
};

class Placar {
    sf::Font fonte;
    sf::Text txtPtosE, txtPtosD;
    int ptosE, ptosD, fontSize;
public:
    Placar() : ptosE(0), ptosD(0), fontSize(40) {
        if(!fonte.loadFromFile("press.ttf")) exit(1);
        txtPtosE.setFont(fonte);
        txtPtosE.setCharacterSize(fontSize);
        txtPtosE.setFillColor(sf::Color::White);
        txtPtosE.setPosition(larguraTela / 4, 10);
        txtPtosD.setFont(fonte);
        txtPtosD.setCharacterSize(fontSize);
        txtPtosD.setFillColor(sf::Color::White);
        txtPtosD.setPosition(larguraTela * 3 / 4, 10);

        txtPtosE.setString(std::to_string(ptosE));
        txtPtosD.setString(std::to_string(ptosD));
    }

    void operator++() {
        ptosE++;
        txtPtosE.setString(std::to_string(ptosE));
    }
    void operator++(int i) {
        ptosD++;
        txtPtosD.setString(std::to_string(ptosD));
    }

    sf::Text txtE() const { return txtPtosE; }
    sf::Text txtD() const { return txtPtosD; }
};

class Jogo {
    sf::RenderWindow janela;
    Raquete esq, dir;
    Bola bola;
    Placar placar;
public:
    Jogo() : janela(sf::VideoMode(larguraTela, alturaTela), "Pong"),
                esq(10, alturaTela / 2 - alturaRaquete / 2),
                dir(larguraTela - larguraRaquete - 10, alturaTela / 2 - alturaRaquete / 2),
                bola(raioBola), placar()
    { run(); }

    void run() {
        sf::Event evento;
        while (janela.isOpen()) {
            while (janela.pollEvent(evento))
                if (evento.type == sf::Event::Closed)
                    janela.close();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                esq.moveUp();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                esq.moveDown();

            dir.moverAutomatica(bola.getY());

            bola.mover();

            if(esq.colide(bola) || dir.colide(bola))
                bola.inverteX();

            if(bola.getX() < 0) {
                placar++;
                bola.resetar();
            }

            if(bola.getX() + raioBola * 2 > larguraTela) {
                ++placar;
                bola.resetar();
            }

            janela.clear(sf::Color::Black);
            janela.draw(esq.shape());
            janela.draw(dir.shape());
            janela.draw(bola.shape());
            janela.draw(placar.txtE());
            janela.draw(placar.txtD());
            janela.display();
        }
    }
};

int main() {
    Jogo j;
    return 0;
}
