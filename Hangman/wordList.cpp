#ifndef WORD_LIST_H
#define WORD_LIST_H

#include <vector>
#include <string_view>

namespace WordList
{
    const std::vector<std::string_view> words
    {
    "apple", "banana", "cherry", "date", "elderberry", "fig", "grape", "honeydew",
    "kiwi", "lemon", "mango", "nectarine", "orange", "papaya", "quince", "raspberry",
    "strawberry", "tangerine", "ugly", "vanilla", "watermelon", "xigua", "yam", "zucchini",
    "airplane", "bicycle", "car", "dolphin", "elephant", "flower", "giraffe", "house",
    "igloo", "jungle", "kangaroo", "lamp", "mountain", "notebook", "ocean", "piano",
    "quartz", "river", "snowflake", "train", "umbrella", "volcano", "whale", "xylophone",
    "yacht", "zebra", "alphabet", "brilliant", "computer", "dinosaur", "elephant",
    "fantastic", "giraffe", "helicopter", "internet", "jigsaw", "kitchen", "lightning",
    "mountain", "notebook", "octopus", "pumpkin", "question", "rainbow", "sunshine",
    "telephone", "umbrella", "violin", "whistle", "xenon", "yellow", "zeppelin",
    "amazing", "beautiful", "courage", "dreamer", "eclipse", "freedom", "gravity",
    "horizon", "illusion", "justice", "knowledge", "laughter", "mystery", "nostalgia",
    "optimism", "paradise", "quality", "respect", "strength", "triumph", "universe",
    "victory", "wisdom", "xenon", "yesterday", "zenith", "gates", 
    "pictogram", "zoom", "waterfall", "arachnid", "technology",
    "information", "computer", "programming", "algorithm", "software",
    "hardware", "keyboard", "monitor", "memory", "processor", "network",
    "contact", "business", "people", "communication", "internet", "website",
    "health", "science", "mathematics", "physics", "chemistry", "biology",
    "geography", "history", "language", "literature", "music", "art",
    "philosophy", "psychology", "sociology", "economics", "politics",
    "religion", "education", "engineering", "medicine", "agriculture",
    "university", "college", "school", "student", "teacher", "professor",
    "research", "experiment", "discovery", "invention", "innovation",
    "development", "progress", "improvement", "success", "failure",
    "challenge", "opportunity", "problem", "solution", "strategy",
    "tactic", "goal", "objective", "target", "mission", "vision",
    "member", "hotel", "church", "village", "town", "city",
    "country", "continent", "ocean", "mountain", "river", "lake",
    "communication", "collaboration", "creation", "design",
    "development", "application", "interface", "database", "architecture",
    "cloud", "security", "analysis", "synthesis", "optimization", "integration",
    "operation", "functionality", "efficiency", "usability", "accessibility",
    "networking", "programmer", "developer", "engineer", "analyst",
    "support", "solution", "consultation", "implementation", "system",
    "platform", "device", "automation", "innovation", "advancement",
    "transformation", "enhancement", "researcher", "scientist", "experimenter",
    "discoverer", "entrepreneur", "businessman", "investor", "leader",
    "manager", "director", "team", "collaborator", "partnership", "organization",
    "administration", "government", "policy", "regulation", "law",
    "treatment", "diagnosis", "symptom", "medicine", "therapy",
    "species", "habitat", "ecosystem", "environment", "biodiversity",
    "geology", "meteorology", "climate", "astronomy", "cosmology",
    "language", "translation", "dialect", "communication", "expression",
    "creativity", "imagination", "inspiration", "motivation", "emotion",
    "experience", "memory", "cognition", "learning", "developmental",
    "community", "culture", "society", "tradition", "heritage",
    "friendship", "relationship", "connection", "support", "compassion",
    "service", "volunteer", "charity", "humanity", "philanthropy",
    "artistry", "craftsmanship", "performance", "theater", "cinema",
    "literacy", "reading", "writing", "expression", "narrative",
    "music", "composition", "performance", "instrument", "melody",
    "rhythm", "harmony", "art", "painting", "sculpture",
    "photography", "design", "fashion", "architecture", "craft"
    };

	std::string_view getRandomWord() { return words[Random::get<std::size_t>(0, words.size() - 1)]; }
}

#endif
