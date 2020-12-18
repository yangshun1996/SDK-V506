package com.fh;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.scheduling.annotation.EnableAsync;

/**
 * Hello world!
 *
 */
@SpringBootApplication
@EnableAsync
public class App 
{
    public static void main(String[] args)  throws InterruptedException
    {
        SpringApplication.run(App.class, args);
    }
}
